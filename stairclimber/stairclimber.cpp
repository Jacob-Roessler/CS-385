/*******************************************************************************
 * Name        : stairclimber.cpp
 * Author      : Jacob Roessler
 * Date        : 9/26/2020
 * Description : Lists the number of ways to climb n stairs.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

vector< vector<int> > get_ways(int num_stairs) {
    vector< vector<int> > ways;
    if(num_stairs <= 0){
        vector<int> temp;
        ways.push_back(temp);
    }
    else{
        for(int i = 1; i <= 3; i++){
            if(num_stairs >= i){
                vector<vector<int>> result = get_ways(num_stairs-i);
                for(int j = 0; j < result.size(); j++){
                    result[j].insert(result[j].begin(), i);
                }
                //cannot use append operation so inserts the contents of result at the end of ways
                ways.insert(ways.end(), result.begin(), result.end());
            }
        }
    }
    return ways;

}
void display_ways(const vector< vector<int> > &ways) {
    // TODO: Display the ways to climb stairs by iterating over
    // the vector of vectors and printing each combination.
    int num_stairs = ways[0].size();
    int num_ways = ways.size();
    cout << num_ways << " ways to climb " << num_stairs << " stairs." << endl;
    
    int digits = 0;
    while(num_ways != 0){
        digits++;
        num_ways /= 10;
    }
    for(int i = 0; i < ways.size(); i++){
        cout << setw(digits) << i+1 << ".";
        cout << " " << "[";
        for(int j = 0; j < ways[i].size(); j++){
            if(j != ways[i].size()-1)
                cout << ways[i][j] << ", ";
            else
                cout<< ways[i][j];
            
        }
        cout << "]" << endl;
    }

}

int main(int argc, char * const argv[]) {
// Unsufficient args
	int num_stairs;
    
        
    //wrong number of args
    if(argc <= 1 || argc >= 2){
		cout << "Usage: ./stairclimber <number of stairs>" << endl;
	}
	else{
		istringstream iss(argv[1]);
		//Check if string is valid input
		if( !(iss >> num_stairs) ){
            cerr << "Error: Number of stairs must be a positive integer." << endl;
        }
        else{
            if(num_stairs < 1)
                cerr << "Error: Number of stairs must be a positive integer." << endl;
            else{
                vector<vector<int>> ways = get_ways(num_stairs);
                display_ways(ways);
            }
            
        }


    }
    return 0;
}

    