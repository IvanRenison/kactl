#include "../utilities/template.h"

#include "../../content/data-structures/SegmentTree2D.h"

struct Slow {
	typedef ll T;
	static constexpr T unit = 0;
	T f(T a, T b) { return a + b; }

	ll n, m;
	vector<vi> a;
	Slow(ll n, ll m) : n(n), m(m), a(n, vi(m)) {}
	void upd(ll x, ll y, T v) {
		a[x][y] = v;
	}
	T query(ll x0, ll x1, ll y0, ll y1) {
		T ans = unit;
		fore(x, x0, x1) fore(y, y0, y1) {
			ans = f(ans, a[x][y]);
		}
		return ans;
	}
};

int main() {

	fore(_, 0, 100) {
		ll n = rand() % 50 + 1, m = rand() % 50 + 1;

		Tree2D st(n, m);
		Slow slow(n, m);

		fore(_, 0, 100) {
			ll t = rand() % 2;
			if (t == 0) {
				ll x = rand() % n, y = rand() % m, val = rand();
				st.upd(x, y, val);
				slow.upd(x, y, val);
			} else {
				ll x0 = rand() % n, x1 = rand() % n;
				if (x0 > x1) swap(x0, x1);
				ll y0 = rand() % m, y1 = rand() % m;
				if (y0 > y1) swap(y0, y1);
				ll ans = st.query(x0, x1, y0, y1);
				ll slowAns = slow.query(x0, x1, y0, y1);
				assert(ans == slowAns);
			}
		}

		ll total = st.query(0, n, 0, m);
		ll total_slow = slow.query(0, n, 0, m);
		assert(total == total_slow);
	}

	cout << "Tests passed!" << endl;
}
