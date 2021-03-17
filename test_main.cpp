#include <iostream>

#include "dmru.h"
#include "smru.h"

int main()
{
	int Mbits;
	int Cbits;
	int Lbits;
	int asoc;
	cin >> Mbits >> Cbits >> Lbits >> asoc;
	dMRU cache = dMRU(Mbits, Cbits, Lbits, asoc);
	ll in;
	while (cin >> in) {
		cout << cache.hit(in) << endl;
		cache.state_dump();
	}
}
