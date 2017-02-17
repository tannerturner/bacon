/* File name: pathfinder.cpp
 */

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

#define MAX 256

using namespace std;


/*
 * Read from a file to get each actor's name, the movie he/she is in and the
 * year, so that we can create a hash table for the actor and the movies. Where 
 * each actor has a list of movies and each movie has a list of actors
 */ 
void createGraph(char * file, ActorGraph * graph) {

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
  }
}



/*This function is for use with the 3rd parameter of the command line arguments
to grab all of the pairs of actors within the file*/
void createPaths(char* in, char* out, ActorGraph * graph, char* flag) {

  ifstream input(in);
  ofstream output(out);

  stack<Movie*> edges = stack<Movie*>();
  stack<Actor*> vertices = stack<Actor*>();

  Actor * tmpVert = 0;
  Movie * tmpEdge = 0;

  string line = "";
  string firstActor = "";
  string secondActor = "";
  const char * actorName = 0;
  const char * movieName = 0;
  const char * movieYear = 0;

  deque<string> allFirstActors = deque<string>();
  deque<string> allSecondActors = deque<string>();  

  string header = "(actor)--[movie#@year]-->(actor)--...\n(";
  string format1 = ")--[";
  string format2 = "#@";
  string format3 = "]-->";

  bool headerWritten = false;

  if(input.good()) {
    input.ignore(256,'\n');

    while(!input.eof()) {

      getline(input, line);

      if(line.empty()) break;

      line += "\t";


      char c[line.size()*2];

      strcpy(c, line.c_str());


      firstActor = strtok(c, "\t");
      secondActor = strtok(0,"\t");

      allFirstActors.push_back(firstActor);
      allSecondActors.push_back(secondActor);
    }
    input.close();
    
    allFirstActors.shrink_to_fit();
    allSecondActors.shrink_to_fit();




    for(int i = 0, size = allFirstActors.size(); i < size; i++) {
      graph->BFS(allFirstActors[i], allSecondActors[i], flag, true);


      vertices = graph->vertices;
      
      
      edges = graph->edges;
      


      while( !edges.empty() ) {
        tmpVert = vertices.top();
        vertices.pop();

        
        tmpEdge = edges.top();
        edges.pop();
        

        actorName = (tmpVert->name).c_str();
        movieName = (tmpEdge->name).c_str();
        movieYear = to_string(tmpEdge->year).c_str();

        if(!headerWritten) {
          output.write(header.c_str(),header.size());
          headerWritten = true;
        } else {
          output.put('(');
        }

        output.write(actorName, strlen(actorName));
        output.write(format1.c_str(), format1.size());

        output.write(movieName, strlen(movieName));
        output.write(format2.c_str(), format2.size());
        
        output.write(movieYear, strlen(movieYear));
        output.write(format3.c_str(), format3.size());

      }
      
      tmpVert = vertices.top();
      actorName = (tmpVert->name).c_str();
      vertices.pop();

      output.put('(');
      output.write(actorName, strlen(actorName));
      output.put(')');
      output.put('\n');

    }
    output.close();

    allFirstActors.clear();
    allSecondActors.clear();

  }
}

int main(int argc, char * argv[]) {
    if(argc == 5) {
      

      ActorGraph * graph = new ActorGraph();
      createGraph(argv[1], graph);
      

      createPaths(argv[3], argv[4], graph, argv[2]);

      delete graph;
      graph = 0;


      return 0;
    
    } else {
      return 1; //wrong number of args
    }
  
}
