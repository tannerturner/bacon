/* File name: Movie.hpp
 */

#ifndef MOVIE_HPP
#define MOVIE_HPP

#include <vector>
#include <string>
#include <set>
#include <deque>
#include <iostream>
#include <functional>

using namespace std;

class Actor;

class Movie {

	public:
		deque<Actor*> actorsIn;
		set<Actor*> unionOfActors;
		string title_year;
		string name;
		int year;
		int index;
		int setsMerged;
		

		Movie() {
            title_year = "";
            name = "";
            year = 0;
            index = 0;
            setsMerged = 0;
            unionOfActors = set<Actor*>();
        }

		Movie(string n, int y): name(n), year(y), index(-1) {
			actorsIn = deque<Actor*>();
			unionOfActors = set<Actor*>();
			actorsIn.clear();
			title_year = "";
			setsMerged = 0;
		}
		
		~Movie();

		void addActorToList(Actor * a);

		int hashCode(string s, int M);

		bool operator<(Movie & other) {
  			return (unionOfActors.size() < other.unionOfActors.size());
		}	

};

#endif // MOVIE_HPP
