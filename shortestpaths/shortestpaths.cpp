/*******************************************************************************
 * Name        : shortestpaths.cpp
 * Author      : Jacob Roessler
 * Date        : 12/1/2020
 * Description : Solves shortest paths problem with Floyd's algorithm.
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
	edge(char s, char e, int w) : start{s}, end{e}, weight{w} {}
	char start, end;
	int weight;
};
//node struct for building the path trees that need to be displayed with data for the from and to points
struct Node
{
    int start, end;
    struct Node* left, *right;
    Node(int start, int end) : start{start}, end{end}, left{nullptr}, right{nullptr} {};
};
//function for figuring out the width of input value for display_table
long len(long max_val){
	if(max_val == 0)
		return 1;
	long digits = 0;
	while(max_val != 0){
		max_val /= 10;
		digits++;
	}
	return digits;
}
void display_table(long** const matrix, const string &label,
					const bool use_letters = false, int const num_vertices = 0) {
	cout << label << endl;
	long max_val = 0;
	for(int i = 0; i < num_vertices; i++){
		for(int j = 0; j < num_vertices; j++){
			long cell = matrix[i][j];
			if (cell != INF && cell > max_val) {
				max_val = matrix[i][j];
			}
		}
	}
	int max_cell_width = use_letters ? len(max_val) :
			len(max(static_cast<long>(num_vertices), max_val));
	cout << ' ';
	for(int j = 0; j < num_vertices; j++) {
		cout << setw(max_cell_width + 1) << static_cast<char>(j + 'A');
	}
	cout << endl;
	for(int i = 0; i < num_vertices; i++) {
		cout << static_cast<char>(i + 'A');
		for(int j = 0; j < num_vertices; j++) {
			cout << " " << setw(max_cell_width);
			if (matrix[i][j] == INF) {
				cout << "-";
			} else if (use_letters) {
				cout << static_cast<char>(matrix[i][j] + 'A');
			} else{
				cout << matrix[i][j];
			}
		}
		cout << endl;
	}
	cout << endl;
}
//Breaks up an input string using ' ' as the delimiter used for extracting each element of input.txt line
vector<string> split(string in){
	vector<string> elements;
	stringstream input(in);
	string temp;

	while(getline(input, temp, ' ')){
		elements.push_back(temp);
	}

	return elements;
}

//Builds the path tree
void buildPathTree(long** const path_lengths, long** const intermediate, Node* root){
	if(intermediate[root->start][root->end] == INF){
		return;
	}
	root->left = new Node(root->start, intermediate[root->start][root->end]);
	root->right = new Node(intermediate[root->start][root->end], root->end);
	buildPathTree(path_lengths, intermediate, root->left);
	buildPathTree(path_lengths, intermediate, root->right);

}
//Performs preorder traversal printing only the roots
void printTree(Node* root){
	if(root  == nullptr){
		return;
	}
	//only print if root
	if(root->left == nullptr && root->right == nullptr)
		cout << static_cast<char>(root->start + 'A') << " -> " ;
	printTree(root->left);
	printTree(root->right);

}
void deleteTree(Node* root){
	if(root == nullptr){
		return;
	}
	deleteTree(root->left);
	deleteTree(root->right);
	delete root;
}
//Builds and traverses path tree to output the path of the given start and end vertex
void getPath(long** const path_lengths, long** const intermediate, int sVert, int eVert){
	Node* root = new Node(sVert, eVert);
	buildPathTree(path_lengths, intermediate, root);
	printTree(root);
	deleteTree(root);
	cout << static_cast<char>(eVert+ 'A');

}

void printPaths(long** const path, long** const intermediate, int size){
	for(int start_vertex = 0; start_vertex < size; start_vertex++){
		for(int end_vertex = 0; end_vertex < size; end_vertex++){
			long distance = path[start_vertex][end_vertex];
			cout << static_cast<char>('A' + start_vertex) << " -> " << static_cast<char>('A' + end_vertex);
			if(distance == 0){
				cout << ", distance: " << distance <<", path: ";
				cout << static_cast<char>('A' + start_vertex);
			}
			else if (distance == INF){
				cout << ", distance: infinity, path: none";
			}
			else{
				cout << ", distance: " << distance <<", path: ";
				getPath(path, intermediate, start_vertex, end_vertex);

			}
			cout << endl;
		}
	}

}
void solveFloyd(long** const distMat, int size){

	long **path = new long*[size];
	for(int r = 0; r < size; r++){
		path[r] = new long[size];
		for(int c = 0; c < size; c++){
			path[r][c] = distMat[r][c];

		}
	}
	long **intermediate = new long*[size];
	for(int r = 0; r < size; r++){
		intermediate[r] = new long[size];
		for(int c = 0; c < size; c++){
			intermediate[r][c] = INF;
		}
	}

	for(int i = 0; i < size; i++){

		for(int r = 0; r < size; r++){
			for(int c = 0; c < size; c++){
				if(r != c && (r != i && c != i)){
					if( path[i][c] + path[r][i] < path[r][c] ){
						path[r][c] = path[i][c] + path[r][i];
						intermediate[r][c] = i;
					}
				}
			}
		}

	}

	display_table(distMat, "Distance matrix:", false, size);
	display_table(path, "Path lengths:", false, size);
	display_table(intermediate, "Intermediate vertices:", true, size);
	printPaths(path, intermediate, size);

	for(int i = 0; i < size; ++i){
			delete [] path[i];
			delete [] intermediate[i];
		}
	delete [] path;
	delete [] intermediate;

}
int main(int argc, char *argv[]){
	istringstream iss;
	string filename;
	if(argc != 2){
		cerr << "Usage: ./shortestpaths <filename>" << endl;
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
	char minVert = 'A';
	char maxVert;
	vector<edge*> edges;
	while(getline (MyReadFile, lineTxt)){
		if(linecount == 1){
			istringstream vert(lineTxt);
			vert >> verticies;
			if(verticies < 1 || verticies > 26){
				cerr << "Error: Invalid number of vertices '" << vert.str() << "' on line 1." << endl;
				return 1;
			}
			maxVert = 'A' + (verticies-1);

		}
		else{
			vector<string> elements = split(lineTxt);
			if(elements.size() != 3){
				cerr << "Error: Invalid edge data '" << lineTxt << "' on line " << linecount << "." << endl;
				return 1;
			}
			char vert1, vert2;
			int weight;
			istringstream v1(elements[0]);
			if(!(v1 >> vert1) || (vert1 < minVert || vert1 > maxVert)){
				cerr << "Error: Starting vertex '" << elements[0] << "' on line " << linecount <<
						" is not among valid values " << minVert << "-" << maxVert << "." << endl;
				return 1;
			}
			istringstream v2(elements[1]);
			if(!(v2 >> vert2) || (vert2 < minVert || vert2 > maxVert)){
				cerr << "Error: Ending vertex '" << elements[1] << "' on line " << linecount <<
						" is not among valid values " << minVert << "-" << maxVert << "." << endl;
				return 1;
			}

			istringstream w(elements[2]);
			if(!(w >> weight) || (weight < 1)){
				cerr << "Error: Invalid edge weight '" << elements[2] << "' on line " << linecount <<
						"." << endl;
				return 1;
			}

			bool duplicate = false;
			for(auto edge : edges){
				if(edge->start == vert1 && edge->end == vert2){
					edge->weight = weight;
					duplicate = true;
				}
			}
			if(!duplicate){
				edge *e = new edge(vert1, vert2, weight);
				edges.push_back(e);
			}
		}
		linecount++;
	}
	MyReadFile.close();
	int size = maxVert-64;
	long **distMat = new long*[size];
	for(int r = 0; r < size; r++){
		distMat[r] = new long[size];
		for(int c = 0; c < size; c++){
			if(r == c){
				distMat[r][c] = 0;
			}
			else{
				distMat[r][c] = INF;
			}
		}
	}
	for(auto e : edges){
		distMat[e->start-65][e->end-65] = e->weight;
	}

	solveFloyd(distMat, size);

	for(auto pointer : edges){
		delete pointer;
	}
	edges.clear();

	for(int i = 0; i < size; ++i){
		delete [] distMat[i];
	}
	delete [] distMat;

	return 0;
}
