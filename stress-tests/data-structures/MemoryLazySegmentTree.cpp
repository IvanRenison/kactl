#include "../utilities/template.h"

#include "../../content/data-structures/MemoryLazySegmentTree.h"

static unsigned R;
ll ra() {
	R *= 791231;
	R += 1231;
	return (ll)(R >> 1);
}

volatile ll res;
int main() {
	ll N = 10;
	vi v(N);
	iota(ALL(v), 0);
	random_shuffle(ALL(v), [](ll x) { return ra() % x; });
	Node* tr = new Node(v,0,N);
	fore(i,0,N) fore(j,0,N) if (i <= j) {
		ll ma = -inf;
		fore(k,i,j) ma = max(ma, v[k]);
		assert(ma == tr->query(i,j));
	}
	fore(it,0,1000000) {
		ll i = ra() % (N+1), j = ra() % (N+1);
		if (i > j) swap(i, j);
		ll x = (ra() % 10) - 5;

		ll r = ra() % 100;
		if (r < 30) {
			::res = tr->query(i, j);
			ll ma = -inf;
			fore(k,i,j) ma = max(ma, v[k]);
			assert(ma == ::res);
		}
		else if (r < 70) {
			tr->add(i, j, x);
			fore(k,i,j) v[k] += x;
		}
		else {
			tr->set(i, j, x);
			fore(k,i,j) v[k] = x;
		}
	}
	cout << "Tests passed!" << endl;
}
