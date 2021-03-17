#ifndef __DMRU_H
	#define __DMRU_H

#include <algorithm>
#include <cmath>
#include <vector>

#include "mru.h"

class dMRU : public MRU {
public:
	// Mbits = bits needed to address memory, so total memory is 2**Mbits.
	// Same with Cbits and Lbits
	// asoc is cache associativity, should be a power of 2.
	dMRU(int Mbits, int Cbits, int Lbits, int asoc) {
		this->Mbits = Mbits;
		this->Cbits = Cbits;
		this->Lbits = Lbits;
		this->asoc = asoc;
		Msize = pow(2,Mbits);
		Csize = pow(2,Cbits);
		Lsize = pow(2,Lbits);
		nlines = Csize / Lsize;
		nblocks = nlines / asoc;
		npossibletags = Msize/nblocks/Lsize;
		block_priorities = vector<vector<ll>>(nblocks, vector<ll>(npossibletags));
		// init cache
		for (int i = 0; i < nblocks; i++) {
			for (int j = 0; j < npossibletags; j++) {
				block_priorities[i][j] = j;
			}
			random_shuffle(block_priorities[i].begin(), block_priorities[i].end());
		}
	}
	bool hit(ll byte) {
		ll nline = byte / Lsize;
		ll nblock = nline % nblocks;
		ll tag = nline / nblocks;
		for (int i = 0; i < npossibletags; i++) {
			if (block_priorities[nblock][i] == tag) {
				// delete this tag and append it to the back, even if there was a cache miss, because why not
				block_priorities[nblock].erase(block_priorities[nblock].begin() + i);
				block_priorities[nblock].push_back(tag);
				return i < asoc;
			}
		}
		// this shouldn't happen
		return false;
	}
	
	void state_dump() {
		for (const auto& block : block_priorities) {
			int i = 0;
			for (const auto& elem : block) {
				cout << elem << " ";
				if (++i == asoc) cout << "| ";
			}
			cout << endl;
		}
	}
	
private:
	int Mbits;
	int Cbits;
	int Lbits;
	int asoc;
	ll nlines;
	ll nblocks;
	ll npossibletags;
	ll Msize;
	ll Csize;
	ll Lsize;
	vector<vector<ll>> block_priorities;
};

#endif 
