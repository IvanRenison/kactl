#include "../utilities/template.h"

#include "../../content/data-structures/LazySegmentTree.h"

// Slow version with the same interface
struct Slow {
	vi elems;
	ll n;
	Slow(ll n) : elems(n), n(n) {}
	Slow(const vi &a) : elems(a), n(SZ(a)) {}
	void upd(ll a, ll b, ll v) {
		fore(i, a, b) {
			elems[i] += v;
		}
	}
	ll query(ll a, ll b) {
		ll ans = 0;
		fore(i, a, b) {
			ans += elems[i];
		}
		return ans;
	}
};

int main() {

	fore(_, 0, 100) {
		ll n = rand() % 99 + 1;
		vi a(n);
		fore(i, 0, n) {
			a[i] = rand() * (rand() % 2 ? 1 : -1);
		}
		Tree st(a);
		assert(st.n == n);
		Slow slow(a);

		assert(st.query(0, n) == slow.query(0, n));

		fore(_, 0, 100) {
			ll t = rand() % 2;
			ll l = rand() % n, r = rand() % n;
			if (l > r) swap(l, r);
			if (t == 0) {
				ll v = rand() * (rand() % 2 ? 1 : -1);
				st.upd(l, r, v);
				slow.upd(l, r, v);
			} else {
				ll ans = st.query(l, r);
				ll slowAns = slow.query(l, r);
				assert(ans == slowAns);
			}
		}
	}

	cout << "Tests passed!" << endl;
}
