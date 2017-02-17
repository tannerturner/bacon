/* File name: Movie.cpp
 */

#include "Movie.hpp"
#include "Actor.hpp"
#include <string.h>

using namespace std;

Movie::~Movie() {
  
    actorsIn.clear();
    unionOfActors.clear();        

  title_year.clear();
  name.clear();
  year = 0;
  index = 0;
}

/*
 * Adds the actor to the current movie's list of actors
 */
void Movie::addActorToList(Actor * a) {
  actorsIn.push_back(a);
  actorsIn.shrink_to_fit();
}

int Movie::hashCode(string s, int M) {  
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
