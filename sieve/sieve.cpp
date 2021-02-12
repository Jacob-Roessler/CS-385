/*******************************************************************************
 * Name        : sieve.cpp
 * Author      : Jacob Roessler
 * Date        : September 13, 2020
 * Description : Sieve of Eratosthenes
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

class PrimesSieve {
public:
    PrimesSieve(int limit);

    ~PrimesSieve() {
        delete [] is_prime_;
    }

    int num_primes() const {
        return num_primes_;
    }

    void display_primes() const;

private:
    // Instance variables
    bool * const is_prime_;
    const int limit_;
    int num_primes_, max_prime_;

    // Method declarations
    int count_num_primes() const;
    void sieve();
    static int num_digits(int num);
};

PrimesSieve::PrimesSieve(int limit) :
        is_prime_{new bool[limit + 1]}, limit_{limit} {
    //initialize array true values
    for(int i = 0; i <= limit; i++ )
    {
    	*(is_prime_ + i ) = true;
    }
    //run sieve
    sieve();
    //find num priems
    num_primes_ = count_num_primes();

    // Loop to find the max prime starting from the end of the is prime array stopping when a prime is found
    for(int i = limit_; i >= 2; i--)
    {
		if( *(is_prime_ + i))
		{
			max_prime_ = i;
			break;
		}
	}
}

void PrimesSieve::display_primes() const {
    // Displays primes using a max width based on the largest prime and a countner to check when a new row is needed
	// uses iomanip to align if there are multiple rows

	cout << "Number of primes found: " << count_num_primes() << endl;
	cout << "Primes up to " << limit_ << ":" << endl;

	const int max_prime_width = num_digits(max_prime_),
			primes_per_row = 80 / (max_prime_width + 1);
	//Counter to be reset each time a new row is reached
	int primes_printed = 0;

	//In the case of 1 row of priumes
	if(count_num_primes() < primes_per_row){
		for(int i = 2; i <= limit_; i ++){
				if (i == max_prime_){
					cout << i;
				}
				else if (*(is_prime_ + i) ){
					cout << i << " ";
				}

			}
	}
	//Otherwise there are multiple rows of primes and there most be a common width
	else{
		for(int i = 2; i <= limit_; i ++){
				if(primes_printed == primes_per_row && i != limit_){
					cout << "\n";
					primes_printed = 0;
				}
				if (*(is_prime_ + i) ){
					primes_printed++;

					if (i == max_prime_ || primes_printed == primes_per_row){
						cout << setw(max_prime_width) << right << i;
					}
					else
						cout << setw(max_prime_width) << right << i << " ";
				}

			}
	}
}

int PrimesSieve::count_num_primes() const {
    // Finds number of primes by iterating through is_prime_ and incrementing a counter

	int num_prime = 0;
	for(int i = 2; i <= limit_; i++){

		if (*(is_prime_ + i) )
		{
			num_prime++;
		}
	}

    return num_prime;
}

void PrimesSieve::sieve() {
    //Utilizes psuedocode from the Eratosthenes sieve to set indexes corresponding to nonprimes in is_prime_ to false
	for(int i = 2; i <= sqrt(limit_); i++){
		if (*(is_prime_ + i) ){
			for(int j = i*i; j <= limit_; j+= i)
				*(is_prime_ + j) = false;
		}
	}

}

int PrimesSieve::num_digits(int num) {
    // Divides by 10 until there is no digits remaining incrementing a digit counter each time
    // Hint: No strings are needed. Keep dividing by 10.
	int digits = 0;
	while(num != 0){
		digits++;
		num /= 10;
	}
    return digits;
}

int main() {
    cout << "**************************** " <<  "Sieve of Eratosthenes" <<
            " ****************************" << endl;
    cout << "Search for primes up to: ";
    string limit_str;
    cin >> limit_str;
    int limit;

    // Use stringstream for conversion. Don't forget to #include <sstream>
    istringstream iss(limit_str);

    // Check for error.
    if ( !(iss >> limit) ) {
        cerr << "Error: Input is not an integer." << endl;
        return 1;
    }
    if (limit < 2) {
        cerr << "Error: Input must be an integer >= 2." << endl;
        return 1;
    }
    // Code for output
    cout << endl;
    // sieve object is placed on stack
    PrimesSieve sieve(limit);
    sieve.display_primes();

    return 0;
}
