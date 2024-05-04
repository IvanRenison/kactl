#include "../utilities/template.h"

#include "../../content/data-structures/SegmentTree2d.h"

struct Slow {
	typedef ll T;
	static constexpr T neut = 0;
	T f(T a, T b) { return a + b; }

	ll n, m;
	vector<vector<T>> a;
	Slow(ll n, ll m) : n(n), m(m), a(n, vector<T>(m)) {}
	void upd(ll x, ll y, T v) {
		a[x][y] = v;
	}
	T query(ll x0, ll x1, ll y0, ll y1) {
		T ans = neut;
		fore(x, x0, x1) fore(y, y0, y1) {
			ans = f(ans, a[x][y]);
		}
		return ans;
	}
};

void testNormal() {
	fore(_, 0, 100) {
		ll n = rand() % 50 + 1, m = rand() % 50 + 1;

		Tree2 st(n, m);
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
}

namespace Modification {

struct Tree2 {
	typedef set<ll> T;
	const T neut = {};
	T f(T a, const T& b) {
		for (ll x : b) {
			a.insert(x);
		}
		return a;
	} // associative & commutative

	ll n, m;
	vector<vector<T>> a, st;
	Tree2(ll n, ll m) : n(n), m(m), a(n, vector<T>(m)),
			st(2 * n, vector<T>(2 * m)) {
		fore(i, 0, n) fore(j, 0, m)
			st[i + n][j + m] = a[i][j];
		fore(i, 0, n) for (ll j = m - 1; j; --j)
			st[i + n][j] = f(st[i + n][2 * j],st[i + n][2 * j + 1]);
		for (ll i = n - 1; i; --i) fore(j, 0, 2 * m)
			st[i][j] = f(st[2 * i][j], st[2 * i + 1][j]);
	}
	void upd(ll x, ll y, T v) {
		st[x + n][y + m] = v;
		for (ll j = y + m; j > 1; j /= 2)
			st[x + n][j / 2] = f(st[x + n][j], st[x + n][j ^ 1]);
		for (ll i = x + n; i > 1; i /= 2)
			for (ll j = y + m; j; j /= 2)
				st[i / 2][j] = f(st[i][j], st[i ^ 1][j]);
	}
	T query(ll x0, ll x1, ll y0, ll y1) { // [x0, x1) * [y0, y1)
		T r = neut;
		ll i0 = x0 + n, i1 = x1 + n;
		for (; i0 < i1; i0 /= 2, i1 /= 2) {
			ll t[4], q = 0;
			if (i0 & 1) t[q++] = i0++;
			if (i1 & 1) t[q++] = --i1;
			fore(k, 0, q) {
				ll j0 = y0 + m, j1 = y1 + m;
				for (; j0 < j1; j0 /= 2, j1 /= 2) {
					if (j0 & 1) r = f(r, st[t[k]][j0++]);
					if (j1 & 1) r = f(r, st[t[k]][--j1]);
				}
			}
		}
		return r;
	}
};

struct Slow {
	typedef set<ll> T;
	const T neut = {};
	T f(T a, const T& b) {
		for (ll x : b) {
			a.insert(x);
		}
		return a;
	}

	ll n, m;
	vector<vector<T>> a;
	Slow(ll n, ll m) : n(n), m(m), a(n, vector<T>(m)) {}
	void upd(ll x, ll y, T v) {
		a[x][y] = v;
	}
	T query(ll x0, ll x1, ll y0, ll y1) {
		T ans = neut;
		fore(x, x0, x1) fore(y, y0, y1) {
			ans = f(ans, a[x][y]);
		}
		return ans;
	}
};

void test() {
	fore(_, 0, 100) {
		ll n = rand() % 50 + 1, m = rand() % 50 + 1;

		Tree2 st(n, m);
		Slow slow(n, m);

		fore(_, 0, 100) {
			ll t = rand() % 2;
			if (t == 0) {
				ll x = rand() % n, y = rand() % m, val = rand();
				set<ll> s = st.query(x, x + 1, y, y + 1);
				set<ll> s_ = slow.query(x, x + 1, y, y + 1);
				assert(s == s_);
				s.insert(val);
				st.upd(x, y, s);
				slow.upd(x, y, s);
			} else {
				ll x0 = rand() % n, x1 = rand() % n;
				if (x0 > x1) swap(x0, x1);
				ll y0 = rand() % m, y1 = rand() % m;
				if (y0 > y1) swap(y0, y1);
				set<ll> ans = st.query(x0, x1, y0, y1);
				set<ll> slowAns = slow.query(x0, x1, y0, y1);
				assert(ans == slowAns);
			}
		}

		set<ll> total = st.query(0, n, 0, m);
		set<ll> total_slow = slow.query(0, n, 0, m);
		assert(total == total_slow);
	}
}

}

int main() {

	testNormal();
	Modification::test();

	cout << "Tests passed!" << endl;
}
