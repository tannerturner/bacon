/* File name: MovieHashTable.hpp
 */

#ifndef MOVIEHASHTABLE_HPP
#define MOVIEHASHTABLE_HPP

#include "Movie.hpp"
#include <string>
#include <set>
#include <iostream>
#include <functional>

using namespace std;

class MoviePtrComp {
	public:
    	bool operator()(Movie* lhs, Movie* rhs) const {
        	return *lhs < *rhs;
    	}
};


class MovieHashTable {
	
	public:
		Movie ** movies;
		int tableSize;
		
		MovieHashTable() {
			movies = 0;
			tableSize = 1;
		}
		
		~MovieHashTable();

		void addMovies(set<string> & movie_year);

		void addMovie(Movie * movie);
		
		Movie * getMovie(string s);
};

#endif // MOVIEHASHTABLE_HPP
