#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include <utility>
#include <functional>

class Graph{
public:
	Graph(const int number_of_nodes);
	void outputDistanceMatrix();
	void createUndirectedEdge(const int node1, const int node2, const float weight);
	void createDirectedEdge(const int start_node, const int destination_node, const float weight);
	bool isEulerian();
	bool isSemiEulerian();
	std::vector<std::vector<float>> getPrimsMST();
	void setGraph(const std::vector<std::vector<float>> vect);
	float getTotalWeight();
	//float getEulerianCycleLength();
	void removeNode(const int node);
	float getClassicalLowerBound();
	float getUpperBound();
private:
	std::vector<std::vector<float>> distance_matrix;
	bool comparePossibleEdges(const std::pair<int, int> &edge1, const std::pair<int, int> &edge2);
};