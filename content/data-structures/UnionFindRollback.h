/**
 * Author: Lukas Polacek, Simon Lindholm
 * Date: 2019-12-26
 * License: CC0
 * Source: folklore
 * Description: Disjoint-set data structure with undo.
 * If undo is not needed, skip st, time() and rollback().
 * Usage: ll t = uf.time(); ...; uf.rollback(t);
 * Time: $O(\log(N))$
 * Status: tested as part of DirectedMST.h
 */
#pragma once

struct RollbackUF {
	vi e; vector<ii> st;
	RollbackUF(ll n) : e(n, -1) {}
	ll size(ll x) { return -e[find(x)]; }
	ll find(ll x) { return e[x] < 0 ? x : find(e[x]); }
	ll time() { return SZ(st); }
	void rollback(ll t) {
		for (ll i = time(); i --> t;)
			e[st[i].fst] = st[i].snd;
		st.resize(t);
	}
	bool join(ll a, ll b) {
		a = find(a), b = find(b);
		if (a == b) return false;
		if (e[a] > e[b]) swap(a, b);
		st.pb({a, e[a]}), st.pb({b, e[b]});
		e[a] += e[b], e[b] = a;
		return true;
	}
};
