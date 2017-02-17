/* File name: ActorGraph.cpp
 */
 #include <queue>

#include "ActorGraph.hpp"

using namespace std;

typedef set<string>::iterator sIterator;

ActorGraph::~ActorGraph() {

	while(!(vertices.empty())) vertices.pop();
  	while(!(edges.empty())) edges.pop();

	if(actors) {
		for(int i=0; i < tableSize; i++) {
			if(actors[i]) {
				delete actors[i];
				actors[i] = 0;
			}
		}
		delete[] actors;
		actors = 0;
	}

	

	tableSize = 0;
}

/*
 * Create the hash table for the movies
 */
void ActorGraph::createMHash(set<string> & movieSet) {

	mHash = MovieHashTable();
	mHash.addMovies(movieSet);
}


/*
 * Create the hash map for actors and add them onto the hash map
 */
void ActorGraph::addActors(deque<string> & actorNames, deque<deque<string>> & llm) {
	tableSize = ceil((double)(actorNames.size()) * 1.3);

	//Approximating next largest prime
	while(   (tableSize % 2 == 0) || (tableSize % 3 == 0) 
		  || (tableSize % 5 == 0) || (tableSize % 7 == 0) ) tableSize++;

	actors = new Actor*[tableSize];
	
	for(int i = 0; i < tableSize; i++)
		actors[i] = 0;

	deque<string> listMoviesActorIn = deque<string>();

	deque<string>::iterator aIt = actorNames.begin();
	deque<string>::iterator aEnd = actorNames.end();

	deque<deque<string>>::iterator llmIt = llm.begin();
	deque<deque<string>>::iterator llmEnd = llm.end();
	
	Actor * a = 0;
	//Store all of the movies the current actor is in
	for(; aIt != aEnd; ++aIt) {
		listMoviesActorIn.clear();
		listMoviesActorIn = *llmIt;
		++llmIt;

		a = new Actor(*aIt);
		
		a->fillMoviesInList(listMoviesActorIn, mHash);
		addActor(a);
	}

	aIt = actorNames.begin();

	for(; aIt != aEnd; ++aIt) {
		(getActor(*aIt))->fillAdjacency();
	}

}


/*
 * Add actor onto the hash table
 */
void ActorGraph::addActor(Actor * actor) {

	int h = actor->hashCode(actor->name, tableSize);

	//If the index isn't empty, do linear probing
	while(actors[h] != 0) {
		h++;
		if(h >= tableSize) {
			h = 0;
		}
	}

	actors[h] = actor;
	actors[h]->index = h;
}

Actor * ActorGraph::getActor(string name) {
	
	Actor * tmp = 0;
	int hash = tmp->hashCode(name, tableSize);
	tmp = actors[hash];
	while(tmp->name != name) {
		hash++;

		if(hash >= tableSize) {
			hash = 0;
		}

		tmp = actors[hash];
	}


	return tmp;
}

/*
 * Creates the path of the actors using Breadth-first search
 */ 
void ActorGraph::BFS(string actorName1, string actorName2, char* weightFlag, bool pathFlag) {
	BFClear(actorName1);
	int edgeWeight = 0, possibleNewDist = 0;
	bool weighted = false;
	if(weightFlag[0] == 'w') {
		weighted = true;
	}

  	Actor * actor1 = getActor(actorName1);
    Actor * actor2 = getActor(actorName2);
  	actor1->dist = 0; //Assign source actor to 0
  	Actor * currentActor = 0;


	deque<Movie*>::iterator movieIt;
	deque<Movie*>::iterator movieEnd;

	deque<Actor*>::iterator actorIt;
	deque<Actor*>::iterator actorEnd;

  	priority_queue<Actor*, deque<Actor*>, ActorPtrComp> actorQ;

  	actorQ.push(actor1);
  	while( !actorQ.empty() ) {
  		currentActor = actorQ.top();
  		actorQ.pop();

  		if( !(currentActor->done) ) {
	  		movieIt = currentActor->moviesIn.begin(); 
	  		movieEnd = currentActor->moviesIn.end();

	  		for(; movieIt != movieEnd; movieIt++) {
	  			edgeWeight = 1;

	  			if(weighted) {
	  				edgeWeight += (2016 - (*movieIt)->year);
	  			}

	  			actorIt = (*movieIt)->actorsIn.begin();
		  		actorEnd = (*movieIt)->actorsIn.end();

		  		for(; actorIt != actorEnd; actorIt++) {
		  			possibleNewDist = edgeWeight + currentActor->dist;
				  	if(possibleNewDist < (*actorIt)->dist ) {

		  				(*actorIt)->dist = possibleNewDist;
		  				(*actorIt)->prev = currentActor->index;
		  				actorQ.push((*actorIt));
		  			}
		  		}
		  	}
		  	currentActor->done = true;	
	 	}
	}
    if(pathFlag) {
        getPath(actor1, actor2);
    }
}


void ActorGraph::getPath(Actor * actor1, Actor * actor2) {

  	deque<Movie*> prevActorMovies = deque<Movie*>();
	deque<Movie*> currActorMovies = deque<Movie*>();
	deque<Movie*> movieIntersection = deque<Movie*>();


	int distActor2 = actor2->dist;


	Actor * currActor = actor2;
	Actor * prevActor = actors[currActor->prev];

	if(distActor2 < INF) {

        while(!(vertices.empty())) vertices.pop();
  		while(!(edges.empty())) edges.pop();

		while(currActor != actor1) {

			movieIntersection.clear();

			prevActorMovies = prevActor->moviesIn;
			currActorMovies = currActor->moviesIn;

	    	sort(prevActorMovies.begin(), prevActorMovies.end());
	    	sort(currActorMovies.begin(), currActorMovies.end());

	    	set_intersection(prevActorMovies.begin(),prevActorMovies.end(),
	    				 	 currActorMovies.begin(),currActorMovies.end(),
	    				 	 back_inserter(movieIntersection));

	    	reverse(prevActorMovies.begin(), prevActorMovies.end());
	    	reverse(currActorMovies.begin(), currActorMovies.end());

	    	vertices.push(currActor);

	    	sort(movieIntersection.begin(), movieIntersection.end());


	    	edges.push(movieIntersection.back());


	    	currActor = prevActor;
	    	if(prevActor != actor1) 
	    		prevActor = actors[prevActor->prev]; //"prevprev is grandparent"	
		}

		vertices.push(actor1);
	}
}

/*
 * Sets all of the distances for the graph to zero and prev to -1
 */
void ActorGraph::BFClear(string actorName1) {

  	Actor * actor1 = getActor(actorName1);  //Source actor
  	actor1->dist = 0; //Assign source actor to 0

  	queue<Actor*> toExplore = queue<Actor*>();
    while(!toExplore.empty()) toExplore.pop();
    toExplore.push(actor1);

  	while(!(vertices.empty())) vertices.pop();
  	while(!(edges.empty())) edges.pop();

  	Actor * next = 0;
  	Actor * neighbor = 0;
  	
	while(!toExplore.empty()) {
		next = toExplore.front();
		toExplore.pop();
		
		set<Actor*>::iterator it = next->adjacencyList.begin();
		set<Actor*>::iterator end = next->adjacencyList.end();

		for( ; it != end; ++it) {
			neighbor = *it;
			
			if(neighbor->dist != INF) {
				neighbor->dist = INF;
				neighbor->prev = -1;
				neighbor->done = false;
				toExplore.push(neighbor);
			}
		}
	}
}
