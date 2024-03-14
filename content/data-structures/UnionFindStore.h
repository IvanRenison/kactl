/**
 * Author: Pietro Palombini
 * Date: 2024-03-01
 * License: CC0
 * Source: folklore
 * Description: Disjoint-set data structure, with support for
 * storing additional data in each set.
 * Time: $O(\alpha(N))$
 * Status: tested on CSES: Road Construction
 */
#pragma once

typedef ll D;
struct UFStore {
	void merge(D& large, const D& small) { large += small; }
	vi e;
	vector<D> d;
	UFStore(ll n) : e(n, -1), d(n) {}
	UFStore(vector<D>& d) : e(SZ(d), -1), d(d) {}
	D& find(ll x) { return d[repr(x)]; }
	ll repr(ll x) { return e[x] < 0 ? x : e[x] = repr(e[x]); }
	bool join(ll a, ll b) {
		a = repr(a), b = repr(b);
		if (a == b) return false;
		if (e[a] > e[b]) swap(a, b);
		e[a] += e[b], e[b] = a, merge(d[a], d[b]);
		return true;
	}
};
