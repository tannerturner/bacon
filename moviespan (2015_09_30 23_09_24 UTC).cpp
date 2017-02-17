/* File name: moviespan.cpp
 */

#include "DisjointSet.hpp"
#include "ActorGraph.hpp"
#include "MovieHashTable.hpp"
#include <string>
#include <set>
#include <algorithm>
#include <vector>
#include <limits>
#include <queue>
#include <deque>
#include <string.h>
#include <iostream>
#include <fstream>


using namespace std;

int createGraph(char * file, ActorGraph * graph) {

    ifstream input(file);

  if( input.good() ) {
    set<string> movieSet = set<string>();
    deque<string> actorList = deque<string>();
    deque<string> moviesActorIn = deque<string>();
    deque<deque<string>> listOfMovLists = deque<deque<string>>();
    
    bool firstActor = true;
    string line = "";
    string actorName = "";
    string title_year = "";

    actorList.clear();
    moviesActorIn.clear();
    listOfMovLists.clear();

    input.ignore(256,'\n');

    while(!input.eof()) {
      getline(input, line);
      if(line.empty()) break;
      line += "\t";

      char c[line.size()*2];
      strcpy(c, line.c_str());

      actorName = strtok(c, "\t");

      if(!firstActor) {
        //This if statement won't execute until a new actor is read from the file
        if(actorName != actorList.back()) {
          actorList.push_back(actorName);
          moviesActorIn.shrink_to_fit(); 
          listOfMovLists.push_back(moviesActorIn);
          moviesActorIn.clear();
        } 
      } else {
          firstActor = false;
          actorList.push_back(actorName);
      }

      title_year = strtok(0,"\t");
      title_year += '#';
      title_year += strtok(0,"\t");
      
      movieSet.insert(title_year);
      moviesActorIn.push_back(title_year);
    }
    input.close();

    listOfMovLists.push_back(moviesActorIn);
    moviesActorIn.shrink_to_fit();
    moviesActorIn.clear();

    actorList.shrink_to_fit();
    listOfMovLists.shrink_to_fit();

    graph->createMHash(movieSet);
    graph->addActors(actorList, listOfMovLists);
    return actorList.size();
  }
}



void movieCover(ActorGraph * graph, DisjointSet * ds, set<Movie*> * m_out) {

  Movie ** movies = (graph->mHash).movies;
  Movie * biggestMovie;
  Actor * unionSet;
  Actor * tmpAct;
  priority_queue<Movie*, deque<Movie*>, MoviePtrComp> moviesSortedBySize;
  deque<Actor*> actorList = deque<Actor*>();
  deque<Actor*>::iterator actorIt;
  deque<Actor*>::iterator actorEnd;
  int counter;
  bool movieNotAlreadyIn;
  

  while(ds->size > 1) {
    moviesSortedBySize = priority_queue<Movie*, deque<Movie*>, MoviePtrComp>();

    for(int i = 0; i < (graph->mHash).tableSize; i++) {

      if( !movies[i] ) continue;
      
      movieNotAlreadyIn = ( m_out->find(movies[i]) == m_out->end() );

      if( movieNotAlreadyIn ) {

        actorList = movies[i]->actorsIn;
        actorIt = actorList.begin();
        actorEnd = actorList.end();

        (movies[i]->unionOfActors).clear();

        for(; actorIt != actorEnd; ++actorIt) {


          tmpAct = ds->find(*actorIt);


          if( ((movies[i])->unionOfActors).insert(tmpAct).second ) {
          }
        }

        
        moviesSortedBySize.push(movies[i]);
        
      }
      
    }
    

    biggestMovie = moviesSortedBySize.top();
    actorList = biggestMovie->actorsIn;
    actorIt = actorList.begin();
    actorEnd = actorList.end();
    counter = 0;

    unionSet = ds->find(*actorIt);
    ++actorIt;


    for(; actorIt != actorEnd; ++actorIt) {

      if( ds->merge(unionSet, (*actorIt)) ) {

        counter++;
      }
    }


    biggestMovie->setsMerged = counter;
    m_out->insert(biggestMovie);
    ds->size -= (biggestMovie->unionOfActors).size() - 1;
  }
}



void writeToFile(char * out, set<Movie*> * m_out) {
  ofstream output(out);

  set<Movie*>::iterator mIt = m_out->begin();
  set<Movie*>::iterator mEnd = m_out->end();

  deque<Actor*>::iterator aIt;
  deque<Actor*>::iterator aEnd;

  string actorName;
  string movieName;
  string movieYear;
  string header = "Actor/Actress\tMovie\tYear\n";

  output.write(header.c_str(), header.size());

  for( ; mIt != mEnd; ++mIt) {
    aIt = ((*mIt)->actorsIn).begin();
    aEnd = ((*mIt)->actorsIn).end();
    
    for( ; aIt != aEnd; ++aIt) {
      actorName = (*aIt)->name;
      movieName = (*mIt)->name;
      movieYear = to_string((*mIt)->year);

      output.write(actorName.c_str(), actorName.size());
      output.put('\t');
      output.write(movieName.c_str(), movieName.size());
      output.put('\t');
      output.write(movieYear.c_str(), movieYear.size());
      output.put('\n');
    }
  }
  output.close();
}


int main(int argc, char * argv[]) {
    if(argc == 3) {
      int size = 0;
      DisjointSet * ds = 0;
      ActorGraph * graph = new ActorGraph();
      set<Movie*> * m_out = new set<Movie*>();
    
      size = createGraph(argv[1], graph);
      
      ds = new DisjointSet(size);
      
      movieCover(graph, ds, m_out);

      writeToFile(argv[2], m_out);

      delete graph;
      graph = 0;
      delete ds;
      ds = 0;
      
      return 0;
    } else {
      return 1; //wrong number of args
    }
  
}
