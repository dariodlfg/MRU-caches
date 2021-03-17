#ifndef __MRU_H
	#define __MRU_H


using namespace std;
typedef long long ll;

class MRU {
public:
	// true if cache hit, false if miss
	virtual bool hit(ll byte) = 0;

	// dump cache state in standard output.
	virtual void state_dump() = 0;
};

#endif 
