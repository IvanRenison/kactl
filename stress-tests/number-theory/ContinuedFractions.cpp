#include "../utilities/template.h"

#include "../../content/number-theory/ContinuedFractions.h"

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
			double x = rand() / (RAND_MAX + 1.0) * 3;
			if (rand() % 2 == 0) x = (rand() % (3*n)) / (double)(rand() % (3*n) + 1);
			auto pa = approximate(x, n);
			auto it = actual.lower_bound(x), it2 = it;
			if (it2 != actual.begin()) --it2;
			auto best =
				min(make_pair(abs(it2->fst - x), it2->snd),
					make_pair(abs(it->fst - x), it->snd)).snd;
			assert(best.fst == pa.fst);
			assert(best.snd == pa.snd);
		}
	}
	cout<<"Tests passed!"<<endl;
	return 0;
}
