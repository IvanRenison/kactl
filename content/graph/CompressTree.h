/**
 * Author: Simon Lindholm
 * Date: 2016-01-14
 * License: CC0
 * Description: Given a tree $T$ rooted at 0, and a subset of nodes $X$ returns
 * tree with vertex set $\{\text{lca}(x, y) : x \in X, y \in X\}$ and edges
 * between every pair of vertices in which one is an ancestor of the other in
 * $T$. Size is at most $2|X| - 1$, including $X$.
 * Returns a list of (par, orig\_index) representing a tree rooted at 0.
 * The root points to itself.
 * Time: $O(|X| \log |X|)$
 * Status: Tested at CodeForces
 */
#pragma once

#include "LCA.h"

typedef vector<ii> vii;
vii compressTree(LCA& lca, const vi& subset) {
	static vi rev; rev.resize(SZ(lca.time));
	vi li = subset, &T = lca.time;
	auto cmp = [&](ll a, ll b) { return T[a] < T[b]; };
	sort(ALL(li), cmp);
	ll m = SZ(li)-1;
	fore(i,0,m) {
		ll a = li[i], b = li[i+1];
		li.pb(lca.lca(a, b));
	}
	sort(ALL(li), cmp);
	li.erase(unique(ALL(li)), li.end());
	fore(i,0,SZ(li)) rev[li[i]] = i;
	vii ret = {{{0, li[0]}}};
	fore(i,0,SZ(li)-1) {
		ll a = li[i], b = li[i+1];
		ret.pb({rev[lca.lca(a, b)], b});
	}
	return ret;
}
