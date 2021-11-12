#include "Graph.h"

Graph::Graph(const int number_of_nodes){
	if (number_of_nodes <= 1) {
		std::cerr << "Too few nodes.";
	}
	else{
		//Initialises distance matrix with -1s
		for (int i = 0; i < number_of_nodes; i++) {
			std::vector<float> vect; //Create vector for row
			distance_matrix.push_back(vect);
			for (int c = 0; c < number_of_nodes; c++) {
				distance_matrix[i].push_back(-1); //Initialise row with -1s
			}
		}
	}
}

void Graph::outputDistanceMatrix(){
	for (unsigned int j = 0; j < distance_matrix.size(); j++) {
		for (unsigned int k = 0; k < distance_matrix[j].size(); k++) {
			std::cout << distance_matrix[j][k] << " ";
		}
		std::cout << "\n";
	}
}

void Graph::createUndirectedEdge(int node1, int node2, float weight){
	if (weight > 0) {
		distance_matrix[node1][node2] = weight;
		distance_matrix[node2][node1] = weight;
	}
	else {
		std::cerr << "Weight must be positive.";
	}
}

void Graph::createDirectedEdge(int start_node, int destination_node, float weight){
	if (weight > 0) {
		distance_matrix[start_node][destination_node] = weight;
	}
	else {
		std::cerr << "Weight must be positive.";
	}
}

bool Graph::isEulerian(){
	for (unsigned int i = 0; i < distance_matrix.size(); i++) {
		int degree = 0;
		for (unsigned int j = 0; j < distance_matrix.size(); j++) {
			if (distance_matrix[i][j] > 0) {
				degree++;
			}
		}
		if (degree % 2 != 0) {
			return false;
		}
	}
	return true;
}

bool Graph::isSemiEulerian(){
	unsigned int odd_nodes = 0;
	for (unsigned int i = 0; i < distance_matrix.size(); i++) {
		int degree = 0;
		for (unsigned int j = 0; j < distance_matrix.size(); j++) {
			if (distance_matrix[i][j] > 0) {
				degree++;
			}
		}
		if (degree % 2 != 0) {
			odd_nodes++;
		}
	}
	if (odd_nodes == 2) {
		return true;
	}
	else {
		return false;
	}
}

std::vector<std::vector<float>> Graph::getPrimsMST(){
	std::vector<std::vector<float>> mst_matrix; //Matrix for MST
	std::vector<int> deleted_rows;
	std::vector<std::pair<int, int>> selected_edges;
	std::vector<int> current_column;
	current_column.push_back(0);
	deleted_rows.push_back(0);

	//Initialise MST matrix
#pragma region
	for (unsigned int i = 0; i < distance_matrix.size(); i++) {
		std::vector<float> vect;
		mst_matrix.push_back(vect);
		for (unsigned int j = 0; j < distance_matrix.size(); j++) {
			mst_matrix[i].push_back(-1);
		}
	}
#pragma endregion

	//Primary algorithm loop
	while (deleted_rows.size() != distance_matrix.size()) { //While not all rows have been deleted
		//deleted_rows.push_back(current_column.back());
		std::vector<std::pair<int, int>> possible_edges;
		for (unsigned int k = 0; k < current_column.size(); k++) { //Get all possible edges from the numbered columns
			for (unsigned int m = 0; m < distance_matrix.size(); m++) {
				possible_edges.push_back(std::make_pair(m, current_column[k]));
			}
		}
		
		std::sort(possible_edges.begin(), possible_edges.end(), std::bind(&Graph::comparePossibleEdges, this, std::placeholders::_1, std::placeholders::_2)); //Sort the possible edges in descending order

		for (unsigned int h = 0; h < possible_edges.size(); h++) {
			bool row_is_deleted = false;
			for (unsigned int l = 0; l < deleted_rows.size(); l++) {
				if (deleted_rows[l] == possible_edges[h].first) {
					row_is_deleted = true;
					break;
				}
			}
			if (distance_matrix[possible_edges[h].first][possible_edges[h].second] == -1 || row_is_deleted == true) {
				continue; //Ignore empty edges and edges in deleted rows
			}
			else {
				selected_edges.push_back(std::make_pair(possible_edges[h].first, possible_edges[h].second)); //Store the lowest weighing edge in the column
				deleted_rows.push_back(possible_edges[h].first);
				current_column.push_back(possible_edges[h].first);
				break;
			}
		}
	}

	//Convert edges into distance matrix form
	for (unsigned int n = 0; n < selected_edges.size(); n++) {
		mst_matrix[selected_edges[n].first][selected_edges[n].second] = distance_matrix[selected_edges[n].first][selected_edges[n].second];
		mst_matrix[selected_edges[n].second][selected_edges[n].first] = distance_matrix[selected_edges[n].first][selected_edges[n].second];
	}
	return mst_matrix;
}

void Graph::setGraph(std::vector<std::vector<float>> vect){
	bool graph_is_valid = true;
	for (unsigned int i = 0; i < vect.size(); i++) {
		for (unsigned int j = 0; j < vect[i].size(); j++) {
			if (vect[i][j] == -1 || vect[i][j] > 0) {
				continue;
			}
			else {
				graph_is_valid = false;
				break;
			}
		}
		if (!graph_is_valid) {
			break;
		}
	}
	if (graph_is_valid) {
		distance_matrix = vect;
	}
}

float Graph::getTotalWeight(){
	float total_weight = 0;
	std::vector<std::pair<int, int>> banned_edges;
	for (unsigned int i = 0; i < distance_matrix.size(); i++) {
		for (unsigned int j = 0; j < distance_matrix[i].size(); j++) {
			if (distance_matrix[i][j] == distance_matrix[j][i]) {
				banned_edges.push_back(std::make_pair(j, i));
			}
			bool is_banned = false;
			for (unsigned int k = 0; k < banned_edges.size(); k++) {
				if (i == banned_edges[k].first && j == banned_edges[k].second) {
					is_banned = true;
					break;
				}
			}
			if (distance_matrix[i][j] != -1 && !is_banned) {
				total_weight += distance_matrix[i][j];
			}
		}
	}
	return total_weight;
}

/*float Graph::getEulerianCycleLength() {
	//Get all odd nodes
#pragma region
	std::vector<int> odd_nodes;
	for (int i = 0; i < distance_matrix.size(); i++) {
		int degree = 0;
		for (int j = 0; j < distance_matrix[i].size(); j++) {
			if (distance_matrix[i][j] > 0) {
				degree++;
			}
		}
		if (degree % 2 != 0) {
			odd_nodes.push_back(i);
		}
	}
#pragma endregion

	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> possible_pairings;
}*/

void Graph::removeNode(int node){
	distance_matrix.erase(distance_matrix.begin() + node);
	for (unsigned int i = 0; i < distance_matrix.size(); i++) {
		distance_matrix[i].erase(distance_matrix[i].begin() + node);
	}
}

float Graph::getClassicalLowerBound(){
	std::vector<float> possible_lower_bounds;
	for (unsigned int node_removed = 0; node_removed < distance_matrix.size(); node_removed++) {
		Graph residual_mst(distance_matrix.size());
		residual_mst.setGraph(this->getPrimsMST());
		residual_mst.removeNode(node_removed);
		std::vector<std::pair<int, int>> removed_edges;
		for (unsigned int i = 0; i < distance_matrix[node_removed].size(); i++) {
			removed_edges.push_back(std::make_pair(node_removed, i));
		}
		std::sort(removed_edges.begin(), removed_edges.end(), std::bind(&Graph::comparePossibleEdges, this, std::placeholders::_1, std::placeholders::_2));
		std::vector<std::pair<int, int>> possible_edges;
		for (unsigned int j = 0; j < removed_edges.size(); j++) {
			if (distance_matrix[removed_edges[j].first][removed_edges[j].second] != -1) {
				possible_edges.push_back(std::make_pair(removed_edges[j].first, removed_edges[j].second));
			}
		}
		std::sort(possible_edges.begin(), possible_edges.end(), std::bind(&Graph::comparePossibleEdges, this, std::placeholders::_1, std::placeholders::_2));
		possible_lower_bounds.push_back(residual_mst.getTotalWeight() + distance_matrix[possible_edges[0].first][possible_edges[0].second] + distance_matrix[possible_edges[1].first][possible_edges[1].second]);
	}
	std::sort(possible_lower_bounds.begin(), possible_lower_bounds.end());
	return possible_lower_bounds[possible_lower_bounds.size() - 1];
}

float Graph::getUpperBound(){
	std::vector<int> start_node;
	start_node.push_back(0);
	std::vector<float> possible_upper_bounds;
	while (start_node.size() != distance_matrix.size() + 1) {
		int current_column = start_node.back();
		std::vector<int> deleted_rows;
		std::vector<std::pair<int, int>> selected_edges;
		deleted_rows.push_back(current_column);
		//Calculates upper bound starting at certain node
		while (deleted_rows.size() != distance_matrix.size()) { //While not all rows have been deleted
			std::vector<std::pair<int, int>> possible_edges;
			for (unsigned int k = 0; k < distance_matrix.size(); k++) { //Get all elements of the current column
				possible_edges.push_back(std::make_pair(k, current_column));
			}
			std::sort(possible_edges.begin(), possible_edges.end(), std::bind(&Graph::comparePossibleEdges, this, std::placeholders::_1, std::placeholders::_2)); //Sort the possible edges in descending order
			for (unsigned int h = 0; h < possible_edges.size(); h++) {
				bool row_is_deleted = false;
				for (unsigned int l = 0; l < deleted_rows.size(); l++) {
					if (deleted_rows[l] == possible_edges[h].first) {
						row_is_deleted = true;
						break;
					}
				}
				if (distance_matrix[possible_edges[h].first][possible_edges[h].second] == -1 || row_is_deleted == true) {
					continue; //Ignore empty edges and edges in deleted rows
				}
				else {
					selected_edges.push_back(std::make_pair(possible_edges[h].first, possible_edges[h].second)); //Store the lowest weighing edge in the column
					deleted_rows.push_back(possible_edges[h].first);
					current_column = possible_edges[h].first;
					break;
				}
			}
		}
		float total = 0;
		for (unsigned int c = 0; c < selected_edges.size(); c++) {
			total += distance_matrix[selected_edges[c].first][selected_edges[c].second];
		}
		total += distance_matrix[selected_edges.back().first][start_node.back()];
		possible_upper_bounds.push_back(total);
		start_node.push_back(start_node.back() + 1);
	}
	std::sort(possible_upper_bounds.begin(), possible_upper_bounds.end());
	return possible_upper_bounds[0];
}

bool Graph::comparePossibleEdges(const std::pair<int, int> &edge1, const std::pair<int, int> &edge2){
	return distance_matrix[edge1.first][edge1.second] < distance_matrix[edge2.first][edge2.second];
}
