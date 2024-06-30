/**
 * Author: Benjamin Qi, Oleksandr Kulkov, chilli
 * Date: 2020-01-12
 * License: CC0
 * Source: https://codeforces.com/blog/entry/53170, https://github.com/bqi343/USACO/blob/master/Implementations/content/graphs%20(12)/Trees%20(10)/HLD%20(10.3).h
 * Description: Decomposes a tree into vertex disjoint heavy paths and light
 * edges such that the path from any leaf to the root contains at most log(n)
 * light edges. Code does additive modifications and sum queries, but can
 * support commutative segtree modifications/queries on paths and subtrees.
 * Takes as input the full adjacency list. VALS\_EDGES being true means that
 * values are stored in the edges, as opposed to the nodes. All values
 * initialized to the segtree default. Root must be 0.
 * Time: O((\log N)^2)
 * Status: stress-tested a but
 */
#pragma once

#include "../data-structures/LazySegmentTree.h"

template <bool VALS_EDGES> struct HLD {
	ll N, tim = 0;
	vector<vi> adj;
	vi par, siz, depth, rt, pos;
	Tree t;
	HLD(vector<vi> adj_)
		: N(SZ(adj_)), adj(adj_), par(N, -1), siz(N, 1), depth(N),
		  rt(N), pos(N), t(N) { dfsSz(0), dfsHld(0); }
	void dfsSz(ll v) {
		if (par[v] != -1) adj[v].erase(find(ALL(adj[v]), par[v]));
		for (ll& u : adj[v]) {
			par[u] = v, depth[u] = depth[v] + 1;
			dfsSz(u);
			siz[v] += siz[u];
			if (siz[u] > siz[adj[v][0]]) swap(u, adj[v][0]);
		}
	}
	void dfsHld(ll v) {
		pos[v] = tim++;
		for (ll u : adj[v]) {
			rt[u] = (u == adj[v][0] ? rt[v] : u);
			dfsHld(u);
		}
	}
	template <class B> void process(ll u, ll v, B op) {
		for (; rt[u] != rt[v]; v = par[rt[v]]) {
			if (depth[rt[u]] > depth[rt[v]]) swap(u, v);
			op(pos[rt[v]], pos[v] + 1);
		}
		if (depth[u] > depth[v]) swap(u, v);
		op(pos[u] + VALS_EDGES, pos[v] + 1);
	}
	void updPath(ll u, ll v, L val) {
		process(u, v, [&](ll l, ll r) { t.upd(l, r, val); });
	}
	T queryPath(ll u, ll v) { // Modify depending on problem
		T res = tneut;
		process(u, v, [&](ll l, ll r) {
				res = f(res, t.query(l, r));
		});
		return res;
	}
	T querySubtree(ll v) { // updSubtree is similar
		return t.query(pos[v] + VALS_EDGES, pos[v] + siz[v]);
	}
};
