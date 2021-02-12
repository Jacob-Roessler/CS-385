/*******************************************************************************
 * Name        : waterjugpuzzle.cpp
 * Author      : Jacob Roessler
 * Date        : 10/11/2020
 * Description : Solves the water jug puzzle.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <sstream>
#include <vector>
#include <queue> 
using namespace std;

struct State {
    int a, b, c;
    string directions;
    State *parent;
    
    State(int _a, int _b, int _c, string _directions) :
        a{_a}, b{_b}, c{_c}, directions{_directions}, parent{nullptr} { }
    
    // String representation of state in tuple form.
    string to_string() {
        ostringstream oss;
        oss << "(" << a << ", " << b << ", " << c << ")";
        return oss.str();
    }
};
bool equals(State* current, State* goal){
    //Function for chcking if states are equal.
    return (current->a == goal->a) & (current->b == goal->b) & (current->c == goal->c);
}

class Waterjug {
public:
    Waterjug(int capA, int capB, int capC, int goalA, int goalB, int goalC);
    
    //free up all new objects created on heap
    ~Waterjug() {
        for(int i = 0; i < capA_+1; ++i){
            for(int j = 0; j < capB_+1; ++j){
                delete ptrs_[i][j]; // delete at [row][col]
            }
            delete[] ptrs_[i]; // delete whole row
        }
        delete [] ptrs_; // delete array
    }

    void display_solution() const;

private:
    // Instance variables
    State ***ptrs_;
    int capA_, capB_, capC_, goalA_, goalB_, goalC_;

    // Method declarations
    int get_solution();
    bool seen(int a, int b, int c);
    string makeDirections(char jugFrom, char jugTo, int amount);
};

Waterjug::Waterjug(int capA, int capB, int capC, int goalA, int goalB, int goalC) :
        ptrs_{new State**[capA+1]}, capA_{capA}, capB_{capB}, capC_{capC}, goalA_{goalA}, goalB_{goalB}, goalC_{goalC}{
    //initialize array with temp states
    for (int i = 0; i < capA+1; ++i) {
        ptrs_[i] = new State*[capB+1];
        for(int j = 0; j < capB+1; ++j){
            State* temp = new State(-1, -1, -1, "temp");
            ptrs_[i][j] = temp; 
        }
    }
    //place initial state in array
    ptrs_[0][0]->a = 0;
    ptrs_[0][0]->b = 0;
    ptrs_[0][0]->c = capC;
    
    //check if jug has solution output accordingly
    if(get_solution()){
        display_solution();
    }
    else{
        cout << "No solution." << endl;
    }
}
//Checks if state with a, b, and c values has been seen
bool Waterjug::seen(int a, int b, int c){
    return (ptrs_[a][b]->a != -1) && (ptrs_[a][b]->b != -1) && (ptrs_[a][b]->c != -1);

}
//Creates the directions given a jug to pour from and to with an amount
string Waterjug::makeDirections(char jugFrom, char jugTo, int amount){
    return "Pour " + to_string(amount) + " gallon" + (amount == 1 ? " " : "s ") + "from " + jugFrom + " to " + jugTo + "." ;

}
//Solves water jug
int Waterjug::get_solution() {
	//create queue of pointers to states
    queue<State *> working_set;
    //push initial state on queue
    working_set.push(ptrs_[0][0]);

    while(!(working_set.empty()) ){
    	//current = front of queue
        State* current = working_set.front();
        working_set.pop();
        
        //Check if the goal is reached return 1 meaning the solution has been found
        if(seen(goalA_, goalB_, goalC_)){
            return 1;
        }
        //otherwise make all possible pours
        else{
        	//Pouring checks if the destination jug is full or if the pour jug is empty
        	//then see how much room the destination jug has left use t
        	//take the min of that and how much is possible to pour to find out the pour amount
        	//if the state the pour arrives at has not yet already been seen, then store it in ptrs[afterpourA][afterpourB]
        	//this probably could have been done in a function to reduce repetition

            //go from c->a
            if( (current->a != capA_) & (current->c != 0) ){
                int room = capA_ - current->a;
                int amount = min(room, current->c);
                int newA = current->a+amount;
                int newC = current->c-amount;
                string instructions = makeDirections('C', 'A', amount);
                if(!seen(newA, current->b, newC)){
                    ptrs_[newA][current->b]->a = newA;
                    ptrs_[newA][current->b]->c = newC;
                    ptrs_[newA][current->b]->directions = instructions;
                    ptrs_[newA][current->b]->parent = ptrs_[current->a][current->b];
                    ptrs_[newA][current->b]->b = current->b;

                    working_set.push(ptrs_[newA][current->b]);
                }
            }
            //go from b->a
            if( (current->a != capA_) & (current->b != 0) ){
                int room = capA_ - current->a;
                int amount = min(room, current->b);
                int newA = current->a + amount;
                int newB = current->b-amount;
                string instructions = makeDirections('B', 'A', amount);
                if(!seen(newA, newB, current->c)){
                    ptrs_[newA][newB]->a = newA;
                    ptrs_[newA][newB]->b = newB;
                    ptrs_[newA][newB]->directions = instructions;
                    ptrs_[newA][newB]->parent = ptrs_[current->a][current->b];
                    ptrs_[newA][newB]->c = current->c;

                    working_set.push(ptrs_[newA][newB]);
                }
                
            }
            //go from c->b
            if( (current->b != capB_) & (current->c != 0) ){
                int room = capB_ - current->b;
                int amount = min(room, current->c);
                int newB = current->b+amount;
                int newC = current->c-amount;
                string instructions = makeDirections('C', 'B', amount);
                if(!seen(current->a, newB, newC)){
                    ptrs_[current->a][newB]->b = newB;
                    ptrs_[current->a][newB]->c = newC;
                    ptrs_[current->a][newB]->directions = instructions;
                    ptrs_[current->a][newB]->parent = ptrs_[current->a][current->b];
                    ptrs_[current->a][newB]->a = current->a;

                    working_set.push(ptrs_[current->a][newB]);
                }

            }
            //go from a->b
            if( (current->b != capB_) & (current->a != 0) ){
                int room = capB_ - current->b;
                int amount = min(room, current->a);
                int newA = current->a-amount;
                int newB = current->b+amount;
                string instructions = makeDirections('A', 'B', amount);
                if(!seen(newA, newB, current->c)){
                    ptrs_[newA][newB]->a = newA;
                    ptrs_[newA][newB]->b = newB;
                    ptrs_[newA][newB]->directions = instructions;
                    ptrs_[newA][newB]->parent = ptrs_[current->a][current->b];
                    ptrs_[newA][newB]->c = current->c;

                    working_set.push(ptrs_[newA][newB]);
                }
                
            }
            //go from b->c
            if( (current->c != capC_) & (current->b != 0) ){
                int room = capC_ - current->c;
                int amount = min(room, current->b);
                int newB = current->b-amount;
                int newC = current->c+amount;
                string instructions = makeDirections('B', 'C', amount);
                if(!seen(current->a, newB, newC)){
                    ptrs_[current->a][newB]->b = newB;
                    ptrs_[current->a][newB]->c = newC;
                    ptrs_[current->a][newB]->directions = instructions;
                    ptrs_[current->a][newB]->parent = ptrs_[current->a][current->b];
                    ptrs_[current->a][newB]->a = current->a;

                    working_set.push(ptrs_[current->a][newB]);

                }
                
            }
            //go from a->c
            if( (current->c != capC_) & (current->a != 0) ){
                int room = capC_ - current->c;
                int amount = min(room, current->a);
                int newA = current->a-amount;
                int newC = current->c+amount;
                string instructions = makeDirections('A', 'C', amount);
                if(!seen(newA, current->b, newC)){
                    ptrs_[newA][current->b]->a = newA;
                    ptrs_[newA][current->b]->c = newC;
                    ptrs_[newA][current->b]->directions = instructions;
                    ptrs_[newA][current->b]->parent = ptrs_[current->a][current->b];
                    ptrs_[newA][current->b]->b = current->b;

                    working_set.push(ptrs_[newA][current->b]);
                }
            }
        }
    }
    return 0;
}

void Waterjug::display_solution() const{
	//find the goal state to trace up from
    State* current = ptrs_[goalA_][goalB_];
    //make vector for pushing string on
    vector<string> dirs;
    cout << "Initial state. (0, 0, " << capC_ << ")" << endl;
    while(current->parent){
    	//this inserts at the start in order to reverse the directions the strings will be output
        dirs.insert(dirs.cbegin(), current->directions + " " + current->to_string());
        current = current->parent;
    }
    //Output directions
    for(auto it = dirs.cbegin(); it != dirs.cend(); ++it){
    	cout << *it + "\n";
    }

}
int main(int argc, char * const argv[]) {
    
    if(argc != 7)
        cout << "Usage: ./waterjugpuzzle <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>" << endl;
    else{
        int capA, capB, capC, goalA, goalB, goalC;
        istringstream iss;
        iss.str(argv[1]);
        if( !(iss >> capA) || (capA < 0)){
            cerr << "Error: Invalid capacity '" << iss.str() << "' for jug A." << endl;
            return 1;
        }
        iss.clear();
        iss.str(argv[2]);
        if( !(iss >> capB) || (capB < 0)){
            cerr << "Error: Invalid capacity '" << iss.str() << "' for jug B." << endl;
            return 1;
        }
        iss.clear();
        iss.str(argv[3]);
        if( !(iss >> capC) || (capC <= 0)){
            cerr << "Error: Invalid capacity '" << iss.str() << "' for jug C." << endl;
            return 1;
        }
        iss.clear();
        iss.str(argv[4]);
        if( !(iss >> goalA) || (goalA < 0)){
            cerr << "Error: Invalid goal '" << iss.str() << "' for jug A." << endl;
            return 1;
        }

        iss.clear();
        iss.str(argv[5]);
        if( !(iss >> goalB) || (goalB < 0)){
            cerr << "Error: Invalid goal '" << iss.str() << "' for jug B." << endl;
            return 1;
        }

        iss.clear();
        iss.str(argv[6]);
        if( !(iss >> goalC) || (goalC < 0)){
            cerr << "Error: Invalid goal '" << iss.str() << "' for jug C." << endl;
            return 1;
        }


        if(goalA > capA){
            cerr << "Error: Goal cannot exceed capacity of jug A." << endl;
            return 1;
        }
        if(goalB > capB){
            cerr << "Error: Goal cannot exceed capacity of jug B." << endl;
            return 1;
        }
        if(goalC > capC){
            cerr << "Error: Goal cannot exceed capacity of jug C." << endl;
            return 1;
        }

        if(goalA + goalB + goalC != capC){
            cerr << "Error: Total gallons in goal state must be equal to the capacity of jug C." << endl;
            return 1;
        }
        iss.clear();
        
        Waterjug jug1(capA, capB, capC, goalA, goalB, goalC);
    }

    return 0;
}
