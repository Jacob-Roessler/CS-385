/*******************************************************************************
 * Name        : powergrid.cpp
 * Author      : Jacob Roessler
 * Date        : 12/13/2020
 * Description : Solves power grid problem by creating a minimum spanning tree.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;

//infinity is defined as unsigned long max
#define INF 4294967295

//edge struct to make extracting input from txt file easier later
struct edge{
	edge(int s, int e, int w, string name) : start{s}, end{e}, weight{w}, name{name} {}
	int start, end;
	long weight;
	string name;
};

//Breaks up an input string using ',' as the delimiter used for extracting each element of input.txt line
vector<string> split(string in){
	vector<string> elements;
	stringstream input(in);
	string temp;

	while(getline(input, temp, ',')){
		elements.push_back(temp);
	}

	return elements;
}
//Performs an insertion sort on a given vector of edges by their string names
void nameSort(vector<edge*>& edges)
{
	size_t j;
	edge* tmp;
	for (size_t i = 1; i < edges.size(); i++){
		j=i;
		tmp=edges[i];
		while (j > 0 && tmp->name.compare(edges[j-1]->name) < 0)
    	{
			edges[j]=edges[j-1];
			j--;
    	}
		edges[j]=tmp;
	}
}
void solveGrid(int size, vector<edge*> edges){
	//Nodes that have been visited
	bool* visited = new bool[size];
	//Edges that will be in the mst
	vector<edge*> mstEdges;
	//Set node 0 (1 in txt file) to be visited since it is the starting point
	visited[0] = true;
	//initialize the rest of visted to false
	for(int i = 1; i < size; i++)
		visited[i] = false;
	//Value of all edges
	long mstSum = 0;
	//Continue to add edges until either there is no solution or there is a path between all vertices
	while(static_cast<int>(mstEdges.size()) < size-1){
		long minEdge = INF;
		edge* used = nullptr;
		//Check all edges to find the minimum valid edge
		//An edge is valid if it connects to an unvisited vertex
		for(auto edge : edges){
			if(edge->weight < minEdge){
				if(!visited[edge->start-1] != !visited[edge->end-1]){
					minEdge = edge->weight;
					used = edge;
				}
			}
		}
		//If an edge is used then add it to the mst and set its endpoints to be visited
		//Otherwise there is no solution because all the vertices are not connected and there are no more valid edges
		if(used != nullptr){
			visited[used->start-1] = visited[used->end-1] = true;
			mstEdges.push_back(used);
			mstSum += used->weight;
		}
		else{
			cout << "No solution." << endl;
			delete [] visited;
			return;
		}


	}
	//Sort edges by their names
	nameSort(mstEdges);
	//Output solution
	cout << "Total wire length (meters): " << mstSum << endl;
	for(auto edge : mstEdges){
		cout << edge->name << " " << "[" << edge->weight << "]" << endl;
	}

	delete [] visited;
}


int main(int argc, char *argv[]){
	istringstream iss;
	string filename;
	if(argc != 2){
		cerr << "Usage: ./powergrid <input file>" << endl;
		return 1;
	}
	else{
		iss.str(argv[1]);
		filename = iss.str();
	}

	if(ifstream(filename)){
	}
	else{
		cerr << "Error: Cannot open file '" << filename << "'." << endl;
		return 1;
	}

	ifstream MyReadFile(filename);
	string lineTxt;
	int verticies;
	int linecount = 1;
	vector<edge*> edges;
	while(getline (MyReadFile, lineTxt)){
		if(linecount == 1){
			istringstream vert(lineTxt);
			vert >> verticies;
			if(verticies < 1 || verticies > 1000){
				cerr << "Error: Invalid number of vertices '" << vert.str() << "' on line 1." << endl;
				return 1;
			}
			if(verticies == 1){
				cout << "No solution." << endl;
				return 0;
			}
		}
		else{
			vector<string> elements = split(lineTxt);
			if(elements.size() != 4){
				cerr << "Error: Invalid edge data '" << lineTxt << "' on line " << linecount << "." << endl;
				return 1;
			}
			int vert1, vert2;
			long weight;
			string name;
			istringstream v1(elements[0]);
			if(!(v1 >> vert1) || (vert1 < 1 || vert1 > verticies)){
				cerr << "Error: Starting vertex '" << elements[0] << "' on line " << linecount <<
						" is not among valid values " << 1 << "-" << verticies << "." << endl;
				return 1;
			}
			istringstream v2(elements[1]);
			if(!(v2 >> vert2) || (vert2 < 1 || vert2 > verticies)){
				cerr << "Error: Ending vertex '" << elements[1] << "' on line " << linecount <<
						" is not among valid values " << 1 << "-" << verticies << "." << endl;
				return 1;
			}

			istringstream w(elements[2]);
			if(!(w >> weight) || (weight < 1)){
				cerr << "Error: Invalid edge weight '" << elements[2] << "' on line " << linecount <<
						"." << endl;
				return 1;
			}
			istringstream n(elements[3]);
			name = n.str();

			bool duplicate = false;
			for(auto edge : edges){
				if(edge->start == vert1 && edge->end == vert2){
					edge->weight = weight;
					duplicate = true;
				}
			}
			if(!duplicate){
				edge *e = new edge(vert1, vert2, weight, name);
				edges.push_back(e);
			}
		}
		linecount++;
	}
	MyReadFile.close();
	int size = verticies;

	solveGrid(size, edges);

	for(auto pointer : edges){
		delete pointer;
	}
	return 0;
}
