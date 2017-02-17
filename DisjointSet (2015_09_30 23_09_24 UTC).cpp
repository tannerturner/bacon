#include "DisjointSet.hpp"

DisjointSet::~DisjointSet() {
  size = 0;
}

Actor * DisjointSet::find(Actor * actor) {
  Actor * curr = actor;


  while(curr->parent) {
    curr = curr->parent;
  }


  return curr;
}

bool DisjointSet::merge(Actor * a, Actor * b) {

  Actor * aSentinel = find(a);


  Actor * bSentinel = find(b);


  bool retval = false;

  if(aSentinel != bSentinel) {
    bSentinel->parent = aSentinel;
    retval = true;
  }

  return retval;
}