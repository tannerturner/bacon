/* File name: MovieHashTable.cpp
 */

#include "MovieHashTable.hpp"
#include <string>
#include <set>
#include <deque>
#include <cmath>
#include <math.h>
#include <string.h>

using namespace std;

typedef set<string>::iterator sIterator;

MovieHashTable::~MovieHashTable() {
	
	if(movies) {
		for(int i=0; i < tableSize; i++) {
			if(movies[i]) {
				delete movies[i];
				movies[i] = 0;
			}
		}

		delete[] movies;
		movies = 0;
	}
	
	tableSize = 0;
}
	/*
	 * Generate a hash table for the movies and add them to to the hash table
	 */
void MovieHashTable::addMovies(set<string> & movieSet) {

	tableSize = ceil((double)(movieSet.size()) * 1.3);

	//Approximating next largest prime
	while(   (tableSize % 2 == 0) || (tableSize % 3 == 0)  || 
			 (tableSize % 5 == 0) || (tableSize % 7 == 0) ) {
		
		tableSize++;	
	} 

	movies = new Movie*[tableSize];
	
	for(int i = 0; i < tableSize; i++) {
		movies[i] = 0;
	}
		

	sIterator it = movieSet.begin();
	sIterator end = movieSet.end();

	Movie * m = 0;
	
	for(; it!=end; ++it) {
		string tmp(*it);
		char c[tmp.size()+1];
		strcpy(c, tmp.c_str());
		string tmpT(strtok(c, "#"));
		string tmpY(strtok(0, "\t"));

		m = new Movie(tmpT, atoi(tmpY.c_str()));
		m->title_year = *it; 
		addMovie(m);
	}
}

/*
 * Adds the current movie onto the hash table by linear probing
 */
void MovieHashTable::addMovie(Movie * movie) {
	int h = movie->hashCode(movie->title_year, tableSize);

	while(movies[h] != 0) {
		h++;
		if(h >= tableSize) {
			h = 0;
		}
	}


	movies[h] = movie;

	movies[h]->index = h;


}

/*
 * Finds the movie in the hash table
 */
Movie * MovieHashTable::getMovie(string title_year) {


	Movie* tmp = 0;

	int hash = tmp->hashCode(title_year, tableSize); 

	tmp = movies[hash];


	while(tmp->title_year != title_year) {
		
		hash++;

		if(hash >= tableSize) {
			hash = 0;
		}

		tmp = movies[hash];

	}
	return tmp;
}
