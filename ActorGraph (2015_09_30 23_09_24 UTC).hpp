/* File name: ActorGraph.hpp
 */
#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include "Actor.hpp"
#include "MovieHashTable.hpp"
#include <string>
#include <set>
#include <deque>
#include <limits>
#include <cmath>
#include <math.h>
#include <queue>
#include <stack>
#include <algorithm>
#include <iostream>
#include <functional>

#define INF numeric_limits<int>::max()

using namespace std;

class ActorPtrComp {
	public:
    	bool operator()(Actor*& lhs, Actor*& rhs) const {
        	return *lhs < *rhs;
    	}
};

class ActorGraph {
	
	public:
		Actor ** actors;
		MovieHashTable mHash;
		stack<Actor*> vertices;
		stack<Movie*> edges;
		int tableSize;
		
		ActorGraph() {
			actors = 0;
			tableSize = 1;

			vertices = stack<Actor*>();
			while(!(vertices.empty())) vertices.pop();

			edges = stack<Movie*>();
			while(!(edges.empty())) edges.pop();
		}

		~ActorGraph();
		
		void addActor(Actor * actor);

		void addActors(deque<string> & actorNames, 
											deque<deque<string>> & llm);
		
        Actor * getActor(string name);

		void createMHash(set<string> & movieSet);

		void BFS(string actorName1, string actorName2, char* weightFlag, bool pathFlag);

		void getPath(Actor * actor1, Actor * actor2);

		void BFClear(string actorName1);
};

#endif // ACTORGRAPH_HPP
