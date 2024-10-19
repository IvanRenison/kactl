// Problem: https://www.spoj.com/problems/MATSUM/
// Status: accepted
#include <bits/stdc++.h>
using namespace std;

#define fst first
#define snd second
#define pb push_back
#define fore(i, a, b) for (ll i = a, gmat = b; i < gmat; i++)
#define ALL(x) x.begin(), x.end()
#define SZ(x) (ll)(x).size()
#define mset(a, v) memset((a), (v), sizeof(a))
typedef int ll;
typedef pair<ll, ll> ii;
typedef vector<ll> vi;

/// content/data-structures/SegmentTree2d.h
struct Tree2 {
	typedef ll T;
	static constexpr T neut = 0;
	T f(T a, T b) { return a + b; } // associative & commutative

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
/// END content

void solveCase() {
	ll N;
	cin >> N;

	Tree2 t(N, N);

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
