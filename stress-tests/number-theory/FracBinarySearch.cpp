#include "../utilities/template.h"

#include "../../content/number-theory/FracBinarySearch.h"

int main() {
	fore(n,1,300) {
		vector<pair<double, ii>> v;
		fore(i,0,n+1) fore(j,1,n+1) if (gcd(i,j) == 1) {
			double r = (double)i / j;
			v.pb({r, {i,j}});
		}
		v.pb({1e9, {0,0}});
		sort(ALL(v));
		map<double, ii> actual(ALL(v));

		fore(iter,0,100000) {
			double x = rand() / (RAND_MAX + 1.0);
			// x *= min(n, 10); // if testing with search range (0, n)
			auto fn = [&](Frac f) { return (double)f.p >= x * (double)f.q; };
			Frac f = fracBS(fn, n);
			auto best = actual.lower_bound(x)->snd;
			assert(best.fst == f.p);
			assert(best.snd == f.q);
		}
	}
	cout << "Tests passed!" << endl;
	return 0;
}
