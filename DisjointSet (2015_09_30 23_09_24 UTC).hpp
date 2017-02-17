#ifndef DISJOINTSET_HPP
#define DISJOINTSET_HPP

#include "Actor.hpp"

class DisjointSet {

public:
  int size;

  DisjointSet() {
    size = 0;
  }

  DisjointSet(int s): size(s) {}

  ~DisjointSet();

  Actor * find(Actor* actor);

  bool merge(Actor * a, Actor * b);

};

#endif // DISJOINTSET_HPP
