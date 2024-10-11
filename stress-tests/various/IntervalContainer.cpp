#include "../utilities/template.h"

#include "../../content/various/IntervalContainer.h"

ll main2() {
	const ll its = 4000000;
	set<ii> iv;
	unsigned r = 1;
	fore(it,0,its) {
		r *= 12391231;
		r += 12312;
		ll m = (r >> 1) % its;
		addInterval(iv, m, m+1);
	}
	return 0;
}

int main() {
	// return main2();
	const ll N = 10;
	const ll iters = 10000000;
	set<ii> iv;
	vi line(N);
	ll sum = 0, sumit = 0;
	fore(it,0,iters) {
		ll r = rand() % 100;
		if (r < 4) {
			iv.clear();
			line.assign(N, 0);
		}
		else if (r < 60) {
			ll l = rand() % (N+1);
			ll r = rand() % (N+1);
			if (l > r) swap(l, r);
			addInterval(iv, l, r);
			fore(i,l,r) line[i] = 1;
		}
		else {
			ll l = rand() % (N+1);
			ll r = rand() % (N+1);
			if (l > r) swap(l, r);
			removeInterval(iv, l, r);
			fore(i,l,r) line[i] = 0;
		}
		ll last = -1;
		ll ct = 0;
		for (auto &pa : iv) {
			assert(0 <= pa.fst && pa.snd <= N);
			assert(pa.fst < pa.snd);
			assert(last < pa.fst);
			fore(i,pa.fst,pa.snd) assert(line[i]);
			last = pa.snd;
			ct += pa.snd - pa.fst;
		}
		fore(i,0,N) ct -= line[i];
		assert(ct == 0);
		sum += SZ(iv);
		++sumit;
	}
	cout << "Tests passed!" << endl;
}
