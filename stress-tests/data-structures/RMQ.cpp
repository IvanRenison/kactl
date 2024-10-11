#include "../utilities/template.h"

#include "../../content/data-structures/RMQ.h"

int main() {
	srand(2);
	fore(N,0,100) {
		vi v(N);
		fore(i,0,N) v[i] = i;
		random_shuffle(ALL(v));
		RMQ<ll> rmq(v);
		fore(i,0,N) fore(j,i+1,N+1) {
			ll m = rmq.query(i,j);
			ll n = 1 << 29;
			fore(k,i,j) n = min(n, v[k]);
			assert(n == m);
		}
	}
	cout << "Tests passed!" << endl;
}
