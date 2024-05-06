/**
 * Author: Iván Renison
 * Date: 2024-03-14
 * License: CC0
 * Source: https://codeforces.com/blog/entry/18051
 * Description: Segment tree with ability to add or set values of large intervals, and compute max of intervals.
 * Can be changed to other things.
 * Ranges are [s, e).
 * Can be changed to other things.
 * Time: O(\log N).
 * Status: stress-tested a bit
 */
#pragma once

const ll inf = 1ll << 60;

struct Tree {
	typedef ll T; // data type
	struct L { ll mset, madd; }; // lazy type
	const T tneut = -inf;     // neutral elements
	const L lneut = {inf, 0};
	T f (T a, T b) { return max(a, b); } // (any associative fn)
	T apply (T a, L b) {
		return b.mset != inf ? b.mset + b.madd : a + b.madd;
	} // Apply lazy
	L comb(L a, L b) {
		if (b.mset != inf) return b;
		return {a.mset, a.madd + b.madd};
	} // Combine lazy

	ll n;
	vector<T> s;
	vector<L> d;

	Tree(ll n = 0) : n(n), s(2*n, tneut), d(n, lneut) {}
	Tree(vector<T>& vals) : n(SZ(vals)), s(2*n), d(n, lneut) {
		copy(ALL(vals), begin(s) + n);
		for (ll i = n - 1; i > 0; i--) s[i] = f(s[2*i], s[2*i+1]);
	}

	void apply_(ll p, L value) {
		s[p] = apply(s[p], value);
		if (p < n) d[p] = comb(d[p], value);
	}

	void build(ll p) {
		while (p > 1)
			p /= 2, s[p] = apply(f(s[2*p], s[2*p+1]), d[p]);
	}

	void push(ll p) {
		for (ll s = 63 - __builtin_clzll(n); s > 0; s--) {
			ll i = p >> s;
			apply_(2*i, d[i]);
			apply_(2*i+1, d[i]);
			d[i] = lneut;
		}
	}

	void upd(ll l, ll r, L value) {
		l += n, r += n;
		push(l);
		push(r - 1);
		ll l0 = l, r0 = r;
		for (; l < r; l /= 2, r /= 2) {
			if (l&1) apply_(l++, value);
			if (r&1) apply_(--r, value);
		}
		build(l0);
		build(r0 - 1);
	}

	T query(ll l, ll r) {
		l += n, r += n;
		push(l);
		push(r - 1);
		T ans = tneut;
		for (; l < r; l /= 2, r /= 2) {
			if (l&1) ans = f(ans, s[l++]);
			if (r&1) ans = f(s[--r], ans);
		}
		return ans;
	}
};
