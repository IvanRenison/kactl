/**
 * Author: Pietro Palombini and Iván Renison
 * Date: 2024-07-25
 * License: CC0
 * Source: folklore
 * Description: Disjoint-set data structure, with undo and support for
 * storing additional data in each component and global data.
 * Default operations and data are for sum in each component and count of components.
 * Time: $O(\alpha(N))$
 * Status: stress tested a bit
 */
#pragma once

struct RollbackSUF {
	typedef ll T; // Global data
	typedef ll D; // Component data
	T ans; // Global data initial value, set in constructor
	void merge(D& large, const D& small) {
		large = large + small, ans--;
	}

	ll n;
	vi e; vector<D> d;
	vector<tuple<ll,ll,ll,D,T>> st;
	RollbackSUF(ll n) : ans(n), n(n), e(n, -1), d(n) {}
	RollbackSUF(vector<D>& d) : n(SZ(d)), ans(n), e(n,-1), d(d){}
	ll size(ll x) { return -e[find(x)]; }
	ll find(ll x) { return e[x] < 0 ? x : find(e[x]); }
	ll time() { return SZ(st); }
	D get(ll x) { return d[find(x)]; }
	void rollback(ll t) {
		while (SZ(st) > t) {
			auto [a, b, s, v, t] = st.back();
			st.pop_back();
			d[a] = v, e[a] -= e[b] = s, ans = t;
		}
	}
	bool join(ll a, ll b) {
		a = find(a), b = find(b);
		if (a == b) return false;
		if (e[a] > e[b]) swap(a, b);
		st.pb({a, b, e[b], d[a], ans});
		merge(d[a], d[b]);
		e[a] += e[b], e[b] = a;
		return true;
	}
};
