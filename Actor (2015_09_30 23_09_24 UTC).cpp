/* File name: Actor.cpp
 */

#include "Actor.hpp"
#include "MovieHashTable.hpp"
#include <string.h>

using namespace std;

typedef set<string>::iterator sIterator;
typedef vector<string>::iterator vIterator;
typedef vector<vector<string>>::iterator vvIterator;

class ActorGraph;
class MovieHashTable;

Actor::~Actor() {
    moviesIn.clear();
    adjacencyList.clear();	
	name.clear();
	index = 0;
	dist = 0;
	prev = 0;
	parent = 0;
}

/*
 * Fills the actor's list of movies by grabbing the movies from the hash tabl
 */
void Actor::fillMoviesInList(deque<string> & movies, MovieHashTable & mht) {

	Movie * m = 0;
	while(!(movies.empty())) {
		m = mht.getMovie(movies.front());
		movies.pop_front();
		moviesIn.push_back(m);

		m->addActorToList(this);
	}
	moviesIn.shrink_to_fit();
}

void Actor::fillAdjacency() {

	deque<Movie*>::iterator mIt = moviesIn.begin();
	deque<Movie*>::iterator mEnd = moviesIn.end();

	deque<Actor*>::iterator aIt;
	deque<Actor*>::iterator aEnd;
	
	for(; mIt != mEnd; ++mIt) {
		aIt = ((*mIt)->actorsIn).begin();
		aEnd = ((*mIt)->actorsIn).end();

		for(; aIt != aEnd; ++aIt) {

			if((*aIt)->name != name && (adjacencyList.insert((*aIt))).second) {
				
				((*aIt)->adjacencyList).insert(this);
			}
		}
	}
}

/*
 * Generate a hash code for an actor
 */
int Actor::hashCode(string s, int M) {

    int intLength = s.length() / 4;
 	int sum = 0;
 	for (int i = 0; i < intLength; i++) {
 		string tmp = s.substr(i * 4, (i * 4) + 4);
 		char c[tmp.length() + 1];
 		strcpy(c,tmp.c_str());
   		
   		int mult = 1;
   		int sz = tmp.length() + 1;
   		for (int j = 0; j < sz; j++) {
 			sum += c[j] * mult;
 			mult *= 256;
   		}
 	}

	string tmp = s.substr(intLength * 4);
 	char c[tmp.length() + 1];
	strcpy(c,tmp.c_str());
 	int mult = 1;
 	int sz = tmp.length() + 1;
 	for (int i = 0; i < sz; i++) {
   		sum += c[i] * mult;
   		mult *= 256;
 	}

 	return(abs(sum) % M);
}	
