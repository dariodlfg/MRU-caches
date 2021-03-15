#ifndef __MRU_H
	#define __MRU_H

#include <random>

using namespace std;
typedef long long ll;

class MRU {
public:
	// true if cache hit, false if miss
	virtual bool hit(ll byte) = 0;
	
	default_random_engine generator;
    uniform_int_distribution<ll> distribution;
};

#endif 
