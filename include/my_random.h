/////////////////////////////////////////////////
// Random number generator class
/////////////////////////////////////////////////
#ifndef _MY_RANDOM
#define _MY_RANDOM

#include <iostream>
#include <random>

using std :: linear_congruential_engine;
using std :: uint_fast32_t;

class RandomNumber{
private:
	linear_congruential_engine<uint_fast32_t, 48271, 0, 2147483647> random_number;
public:
	////////////////////////////////////////////////////////////////
	// Define the generator variable
	////////////////////////////////////////////////////////////////
	void initialize(double seed){
		random_number.seed(seed);
	}
	////////////////////////////////////////////////////////////////
	// Generate the random number
	////////////////////////////////////////////////////////////////
	double get(double range_low, double range_high){
		double raw_value, min_val, max_val, result;

		min_val = random_number.min();
		max_val = random_number.max();

		raw_value = random_number();

		result = (raw_value - min_val) / (max_val - min_val);
		
		return result;
	}
};

#endif
