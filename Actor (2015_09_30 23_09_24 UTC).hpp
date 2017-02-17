/* File name: Actor.hpp
 */

#ifndef ACTOR_HPP
#define ACTOR_HPP

#include <string>
#include <set>
#include <cmath>
#include <math.h>
#include <limits>
#include <vector>
#include <deque>
#include <iostream>
#include <functional>
#include <algorithm>

using namespace std;

#define INF numeric_limits<int>::max()

class Movie;
class MovieHashTable;

class Actor {

	public:
		deque<Movie*> moviesIn;
		set<Actor*> adjacencyList;
		Actor * parent;
		string name;
		int index;
		int dist;
		int prev;
		bool done;
		
		Actor() {
			done = false;
            name = "";
            index = -1;
            dist = INF;
            prev = -1;
            parent = 0;
        }

		Actor(string n): name(n), index(-1), dist(INF), prev(-1){
			moviesIn = deque<Movie*>();
			adjacencyList = set<Actor*>();
			moviesIn.clear();
			adjacencyList.clear();
			done = false;
			parent = 0;
		}
		
		~Actor();

		int hashCode(string s, int M);

		void fillMoviesInList(deque<string> & movies, MovieHashTable & mht);

		void fillAdjacency();

		bool operator<(Actor & other) {
  			if(dist != other.dist) {
  				return dist > other.dist;
  			} else {
	  			return moviesIn.size() < other.moviesIn.size();
  			}
		}
};

#endif // ACTOR_HPP
