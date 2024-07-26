/**
 * Author: Iván Renison
 * Date: 2024-07-25
 * License: CC0
 * Source: notebook el vasito
 * Description: Offline disjoint-set data structure with remove of arbitrary edges.
 * Uses UnionFindRollbackStore so it also supports queries of global value of RSUF.
 * First use add, remove and query to make operations and then call process to get
 * the answers of the queries in the variable ans.
 * Time: O(Q \log^2 N)
 * Status: stress tested a bit and tested in codeforces
 */
#pragma once

#include "../data-structures/UnionFindRollbackStore.h"

enum { ADD, DEL, QUERY };
struct Query { ll type, x, y; }; // You can add stuff for QUERY
struct DynCon {
	vector<Query> q;
	RSUF uf;
	vi mt;
	map<ii, ll> last;
	vector<T> ans;
	DynCon(ll n) : uf(n) {}
	DynCon(vector<D>& d) : uf(d) {}
	void add(ll x, ll y) {
		if (x > y) swap(x, y);
		mt.pb(-1);
		last[{x, y}] = SZ(q);
		q.pb({ADD, x, y});
	}
	void remove(ll x, ll y) {
		if (x > y) swap(x, y);
		ll pr = last[{x, y}];
		mt[pr] = SZ(q);
		mt.pb(pr);
		q.pb({DEL, x, y});
	}
	void query() { // Add parameters if needed
		q.pb({QUERY, -1, -1});
		mt.pb(-1);
	}
	void process() { // Answers all queries in order
		if (q.empty()) return;
		fore(i, 0, SZ(q))
			if (q[i].type == ADD && mt[i] < 0) mt[i] = SZ(q);
		go(0, SZ(q));
	}
	void go(ll s, ll e) {
		if (s + 1 == e) {
			if (q[s].type == QUERY) { // Answer query using DSU
				ans.pb(uf.ans); // Maybe you want to use uf.get(x)
			}                 // for some x stored in Query
			return;
		}
		ll k = uf.time(), m = (s + e) / 2;
		for (ll i = e; --i >= m;) {
			if (0 <= mt[i] && mt[i] < s) uf.join(q[i].x, q[i].y);
		}
		go(s, m);
		uf.rollback(k);
		for (ll i = m; --i >= s;)
			if (mt[i] >= e) uf.join(q[i].x, q[i].y);
		go(m, e);
		uf.rollback(k);
	}
};
