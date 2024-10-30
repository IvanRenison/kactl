/**
 * Author: Iván Renison
 * Date:
 * License: CC0
 * Source: notebook el vasito
 * Description: Query sum of area ans make point updates.
 * Bounds are inclusive to the left and exclusive to the right.
 * Can be changed by modifying T, f and unit.
 * Time: O(\log N)
 * Status: Tested on SOPJ MATSUM and stress-tested a bit
 */
#pragma once

struct Tree2 {
	typedef ll T;
	static constexpr T neut = 0;
	T f(T a, T b) { return a + b; } // associative & commutative

	ll n, m;
	vector<vector<T>> a, st;
	Tree2(ll n, ll m) : n(n), m(m), a(n, vector<T>(m)),
			st(2 * n, vector<T>(2 * m)) {
		fore(i, 0, n) fore(j, 0, m) st[i + n][j + m] = a[i][j];
		fore(i, 0, n) for (ll j = m; --j;)
			st[i + n][j] = f(st[i + n][2 * j], st[i + n][2 * j + 1]);
		for (ll i = n; --i;) fore(j, 0, 2 * m)
			st[i][j] = f(st[2 * i][j], st[2 * i + 1][j]);
	}
	void upd(ll x, ll y, T v) {
		st[x + n][y + m] = v;
		for (ll j = y + m; j > 1; j /= 2)
			st[x + n][j / 2] = f(st[x + n][j], st[x + n][j ^ 1]);
		for (x += n; x > 1; x /= 2) for (ll j = y + m; j; j /= 2)
			st[x / 2][j] = f(st[x][j], st[x ^ 1][j]);
	}
	T query(ll x0, ll x1, ll y0, ll y1) { // [x0, x1) * [y0, y1)
		T r = neut;
		for (x0 += n, x1 += n; x0 < x1; x0 /= 2, x1 /= 2) {
			ll t[4], q = 0;
			if (x0 & 1) t[q++] = x0++;
			if (x1 & 1) t[q++] = --x1;
			fore(k, 0, q)
				for (ll j0 = y0+m, j1 = y1+m; j0 < j1; j0/=2, j1/=2) {
					if (j0 & 1) r = f(r, st[t[k]][j0++]);
					if (j1 & 1) r = f(r, st[t[k]][--j1]);
				}
		}
		return r;
	}
};
