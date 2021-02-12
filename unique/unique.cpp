/*******************************************************************************
 * Name        : unique.cpp
 * Author      : Jacob Roessler
 * Date        : 09/22/2020
 * Description : Determining uniqueness of chars with int as bit vector.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <cctype>

using namespace std;

bool is_all_lowercase(const string &s) {
    // Returns true if all characters in string are lowercase
    // letters in the English alphabet; false otherwise.
	// If a character does not fall under the ascii range (97,122) then it is not a valid lowercase character
	for(unsigned int i = 0; i < s.length(); i++){
		if(s[i] < 97 || s[i] > 122)
			return false;
	}

	return true;
}

bool all_unique_letters(const string &s) {
    // Returns true if all letters in string are unique, that is
    // no duplicates are found; false otherwise.
    // You may use only a single int for storage and work with bitwise
    // and bitshifting operators.
    // No credit will be given for other solutions.
	unsigned int letterVector = 0;
	unsigned int setter;

	for(unsigned int i = 0; i < s.length(); i++){
		// Setter shifts a 1 to the distance from the first position 'a'
		setter = 1 << (s[i] - 'a');
		//Checks if the letter has already been seen terminates if true
		if( (letterVector & setter) != 0)
			return false;
		//Stores current character in letterVector
		letterVector = letterVector | setter;

	}

	return true;
}

int main(int argc, char * const argv[]) {
    // Reads and parses command line arguments.
    // Calls other functions to produce correct output.

	// Unsufficient args
	if(argc <= 1){
		cout << "Usage: ./unique <string>" << endl;
	}
	// Too many args
	else if(argc > 2){
		cout << "Usage: ./unique <string>" << endl;
	}
	// For single string args
	else{
		string word(argv[1]);
		//Check if string is valid
		if(!is_all_lowercase(word))
			cout << "Error: String must contain only lowercase letters." << endl;
		else{
			//Perform check to see if letters are uniqeu
			if(all_unique_letters(word))
				cout << "All letters are unique." << endl;
			else
				cout << "Duplicate letters found." << endl;
		}
	}

}
