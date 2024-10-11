#include "../utilities/template.h"

#include "../../content/data-structures/FenwickTree.h"

int main() {
	fore(it,0,100000) {
		ll N = rand() % 10;
		FT fw(N);
		vi t(N);
		fore(i,0,N) {
			ll v = rand() % 3;
			fw.upd(i, v);
			t[i] += v;
		}
		ll q = rand() % 20;
		ll ind = fw.lower_bound(q);
		ll res = -1, sum = 0;
		fore(i,0,N+1) {
			if (sum < q) res = i;
			if (i != N) sum += t[i];
		}
		assert(res == ind);
	}
	cout << "Tests passed!" << endl;
}
