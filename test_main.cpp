#include <iostream>

#include "smru.h"

int main()
{
	int Mbits;
	int Cbits;
	int Lbits;
	int asoc;
	cin >> Mbits >> Cbits >> Lbits >> asoc;
	sMRU cache = sMRU(Mbits, Cbits, Lbits, asoc);
	ll in;
	while (cin >> in) {
		cout << cache.hit(in) << endl;
	}
}
