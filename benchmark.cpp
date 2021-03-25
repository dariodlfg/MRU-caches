#include <cstdlib>
#include <ctime>
#include <iostream>

#include "dmru.h"
#include "smru.h"

void simulateMatrixProduct(int msize, int batchsize, int vsize_linealigned, ll position_matrix, ll position_vector, MRU* cache, ll& nHits, ll& nAccesses) {
	for (int i = 0; i < msize; i++) {
		for (int j = 0; j < msize; j++) {
			for (int k = 0; k < batchsize; k++) {
				// output[i][k] = m[i][j]*v[j][k]
				// m[i][j]
				nHits += cache->hit(position_matrix + 2*(i*msize+j));
				// v[j][k]
				nHits += cache->hit(position_vector + 2*(j*batchsize+k));
				// output[i][k]
				nHits += cache->hit(position_vector + vsize_linealigned + 2*(i*batchsize+k));
				nAccesses += 3;
			}
		}
	}
}

void simulateNonLinearOperations(int msize, int batchsize, ll position_vector, MRU* cache, ll& nHits, ll& nAccesses) {
	for (int i = 0; i < msize; i++) {
		for (int j = 0; j < batchsize; j++) {
			nHits += cache->hit(position_vector + 2*(i*batchsize+j));
			nAccesses++;
		}
	}
	
}

int main()
{
	while (0, 1) {
		int Mbits;
		int Cbits;
		int Lbits;
		int asoc;
		cout << "Mbits, Cbits, Lbits, asoc, isdMRU?: ";
		bool isdmru;
		cin >> Mbits >> Cbits >> Lbits >> asoc >> isdmru;
		int msize;
		cout << "matrix size?: ";
		cin >> msize;
		int nlayers;
		cout << "nlayers?: ";
		cin >> nlayers;
		int batchsize;
		cout << "batch size?: ";
		cin >> batchsize;
		int nreps;
		cout << "nreps?: ";
		cin >> nreps;
		// Matrix locations can be deterministic, since cache setup is already randomized.
		// matrices should be line aligned, because we're civilized people
		// The *2 is because we're using float16
		int Lsize = pow(2, Lbits);
		ll msize_linealigned = ceil(double((msize*msize*2))/double(Lsize))*Lsize;
		const ll startingpoint_vectors = msize_linealigned * nlayers;
		ll vsize_linealigned = ceil(double(msize*batchsize*2)/double(Lsize))*Lsize;
		cout << msize_linealigned << " " << startingpoint_vectors << " " << vsize_linealigned << endl << endl;
		
		const int nRetries = 20;
		vector<ll> nHits(nRetries);
		vector<ll> nAccesses(nRetries);
		for (int ii = 0; ii < nRetries; ii++) {
			MRU* cache = isdmru ? (MRU*) new dMRU(Mbits, Cbits, Lbits, asoc) : (MRU*) new sMRU(Mbits, Cbits, Lbits, asoc);
			for (int i = 0; i < nreps; i++) {
				for (int j = 0; j < nlayers; j++) {
					simulateMatrixProduct(msize, batchsize, vsize_linealigned, j*msize_linealigned, startingpoint_vectors+j*vsize_linealigned, cache, nHits[ii], nAccesses[ii]);
					simulateNonLinearOperations(msize, batchsize, startingpoint_vectors+(j+1)*vsize_linealigned, cache, nHits[ii], nAccesses[ii]);
				}
			}
			cout << "*";
		}
		cout << endl;
		cout << "# of accesses: " << nAccesses[0] << endl;
		cout << "# of hits: " << endl;
		for (const auto& hits : nHits) {
			cout << hits << " ";
		}
		cout << endl;
		double avghits=0,stddevhits=0,avgrate=0,stddevrate=0;
		for (const auto& hits : nHits) {
			avghits += hits;
			stddevhits += hits*hits;
			double hitrate = double(hits)/double(nAccesses[0]);
			avgrate += hitrate;
			stddevrate += hitrate*hitrate;
		}
		avghits /= nRetries;
		stddevhits /= nRetries - 1;
		avgrate /= nRetries;
		stddevrate /= nRetries - 1;
		stddevhits = sqrt(stddevhits);
		stddevrate = sqrt(stddevrate);
		cout << "Hits: " << endl;
		cout << "Avg: " << avghits << endl << "Stddev: " << stddevhits << endl;
		cout << "Hit rate: " << endl;
		cout << "Avg: " << avgrate << endl << "Stddev: " << stddevrate << endl;
	}
}
