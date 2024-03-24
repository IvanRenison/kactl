// Problem: https://www.spoj.com/problems/MATSUM/
// Status: accepted
#include <bits/stdc++.h>
using namespace std;

#define fst first
#define snd second
#define pb push_back
#define fore(i, a, b) for (ll i = a, gmat = b; i < gmat; i++)
#define ALL(x) begin(x), end(x)
#define SZ(x) (ll)(x).size()
#define mset(a, v) memset((a), (v), sizeof(a))
typedef long long ll;
typedef pair<ll, ll> ii;
typedef vector<ll> vi;

/// content/data-structures/SegmentTree2S.cpp
struct Tree2D {
	typedef ll T;
	static constexpr T unit = 0;
	T f(T a, T b) { return a + b; }

	ll n, m;
	vector<vi> a, st;
	Tree2D(ll n, ll m) : n(n), m(m), a(n, vi(m)), st(2 * n, vi(2 * m)) {
		fore(i, 0, n)
				fore(j, 0, m)
						st[i + n][j + m] = a[i][j];
		fore(i, 0, n) for (ll j = m - 1; j; --j)
				st[i + n][j] = f(st[i + n][j << 1], st[i + n][j << 1 | 1]);
		for (ll i = n - 1; i; --i)
			fore(j, 0, 2 * m)
					st[i][j] = f(st[i << 1][j], st[i << 1 | 1][j]);
	}
	void upd(ll x, ll y, T v) {
		st[x + n][y + m] = v;
		for (ll j = y + m; j > 1; j >>= 1)
			st[x + n][j >> 1] = f(st[x + n][j], st[x + n][j ^ 1]);
		for (ll i = x + n; i > 1; i >>= 1)
			for (ll j = y + m; j; j >>= 1)
				st[i >> 1][j] = f(st[i][j], st[i ^ 1][j]);
	}
	T query(ll x0, ll x1, ll y0, ll y1) {
		T r = unit;
		for (ll i0 = x0 + n, i1 = x1 + n; i0 < i1; i0 >>= 1, i1 >>= 1) {
			ll t[4], q = 0;
			if (i0 & 1)
				t[q++] = i0++;
			if (i1 & 1)
				t[q++] = --i1;
			fore(k, 0, q) for (ll j0 = y0 + m, j1 = y1 + m; j0 < j1; j0 >>= 1, j1 >>= 1) {
				if (j0 & 1)
					r = f(r, st[t[k]][j0++]);
				if (j1 & 1)
					r = f(r, st[t[k]][--j1]);
			}
		}
		return r;
	}
};
// END content

void solveCase() {
	ll N;
	cin >> N;

	Tree2D t(N, N);

	while (true) {
		string cmd;
		cin >> cmd;
		if (cmd == "SET") {
			ll x, y, num;
			cin >> x >> y >> num;
			t.upd(x, y, num);
		} else if (cmd == "SUM") {
			ll x0, y0, x1, y1;
			cin >> x0 >> y0 >> x1 >> y1;
			ll ans = t.query(x0, x1 + 1, y0, y1 + 1);
			cout << ans << '\n';
		} else {
			break;
		}
	}
}

int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll T;
	cin >> T;

	fore(_, 0, T) {
		solveCase();
	}
}
