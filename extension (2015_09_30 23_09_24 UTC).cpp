/* File name: extension.cpp
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

using namespace std;

void createPaths(ActorGraph * graph, deque<string> & actors);

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

    moviesActorIn.shrink_to_fit();
    listOfMovLists.push_back(moviesActorIn);
    moviesActorIn.clear();

    actorList.shrink_to_fit();
    listOfMovLists.shrink_to_fit();

    graph->createMHash(movieSet);
    graph->addActors(actorList, listOfMovLists);
    
    createPaths(graph, actorList);
    }
}


void createPaths(ActorGraph * graph, deque<string> & actors) {
    string KB = "BACON, KEVIN (I)";
    int totalLength = 0, numOfActors = actors.size(), numActorsNotConnected = 0;
    int thisWeight = 0;
    double avgPathLength = 0.0;
    char c[2] = "u";
    
    cout<<"in create paths"<<endl;
    
    graph->BFS(KB, KB, c, false);

    deque<string>::iterator it = actors.begin();
    deque<string>::iterator end = actors.end();

    for(; it != end; ++it) {
 
      thisWeight = (graph->getActor(*it))->dist;
        
      if( thisWeight == 0 || thisWeight == INF ) {
        numActorsNotConnected++;
      } else {
        totalLength += thisWeight;
      }
    }

    numOfActors -= numActorsNotConnected;
    avgPathLength = totalLength/(double)numOfActors;
    cout<<"Average path length to Kevin Bacon is: "<<avgPathLength<<endl;
}


int main(int argc, char * argv[]) {
      ActorGraph * graph = new ActorGraph();
    
      createGraph(argv[1], graph);
    
      delete graph;
      graph = 0;
          
      return 0;
  
}
