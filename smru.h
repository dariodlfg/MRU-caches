#ifndef __SMRU_H
	#define __SMRU_H

#include <cmath>
#include <ctime>
#include <set>
#include <vector>

#include "mru.h"

class sMRU : public MRU {
public:
	// Mbits = bits needed to address memory, so total memory is 2**Mbits.
	// Same with Cbits and Lbits
	// asoc is cache associativity, should be a power of 2.
	sMRU(int Mbits, int Cbits, int Lbits, int asoc) {
		this->Mbits = Mbits;
		this->Cbits = Cbits;
		this->Lbits = Lbits;
		this->asoc = asoc;
		Msize = pow(2,Mbits);
		Csize = pow(2,Cbits);
		Lsize = pow(2,Lbits);
		nlines = Csize / Lsize;
		nblocks = nlines / asoc;
		cache = vector<vector<ll>>(nblocks, vector<ll>(asoc));
		// init cache
		generator = default_random_engine(time(NULL));
		distribution = uniform_int_distribution<ll>(0, Msize/nblocks/Lsize);
		for (int i = 0; i < nblocks; i++) {
			set<ll> used_blocks;
			for (int j = 0; j < asoc ; j++) {
				ll temp;
				do {
					temp = distribution(generator);
				} while (!used_blocks.insert(temp).second);
				cache[i][j] = temp;
			}
		}
		
	}
	bool hit(ll byte) {
		ll nline = byte / Lsize;
		ll nblock = nline % nblocks;
		ll tag = nline / nblocks;
		for (ll& cache_tag : cache[nblock]) {
			if (cache_tag == tag) {
				// hit!
				set<ll> used_blocks;
				for (ll temp_tag : cache[nblock]) used_blocks.insert(temp_tag);
				ll temp;
				do {
					temp = distribution(generator);
				} while (!used_blocks.insert(temp).second);
				cache_tag = temp;
				return true;
			} 
		}
		return false;
	}
	
private:
	int Mbits;
	int Cbits;
	int Lbits;
	int asoc;
	ll nlines;
	ll nblocks;
	ll Msize;
	ll Csize;
	ll Lsize;
	vector<vector<ll>> cache;
};

#endif 
