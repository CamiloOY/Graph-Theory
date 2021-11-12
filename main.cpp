#include <iostream>
#include "Graph.h"

using namespace std;

int main() {
	Graph graph(5);
	graph.createUndirectedEdge(0, 1, 8);
	graph.createUndirectedEdge(0, 2, 7);
	graph.createUndirectedEdge(0, 3, 29);
	graph.createUndirectedEdge(0, 4, 13);
	graph.createUndirectedEdge(1, 2, 9);
	graph.createUndirectedEdge(1, 3, 24);
	graph.createUndirectedEdge(1, 4, 14);
	graph.createUndirectedEdge(2, 3, 23);
	graph.createUndirectedEdge(2, 4, 6);
	graph.createUndirectedEdge(3, 4, 21);

	graph.outputDistanceMatrix();
	cout << "\n" << graph.getUpperBound();
	return 0;
}