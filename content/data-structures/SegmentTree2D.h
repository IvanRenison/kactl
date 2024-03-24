/**
 * Author: Iván Renison
 * Date:
 * License: CC0
 * Source: notebook el vasito
 * Description:
 * Time:
 * Status:
 */
#pragma once

struct Tree2D {
	typedef ll T;
	static constexpr T unit = LONG_LONG_MIN;
	T f(T a, T b) { return max(a, b); }

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
