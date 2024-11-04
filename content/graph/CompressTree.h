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
 * Status: stress tested
 */
#pragma once

#include "LCA.h"

vector<ii> compressTree(LCA& lca, vi X) {
	static vi rev; rev.resize(SZ(lca.time));
	auto cmp = [&](ll a,ll b){ return lca.time[a]<lca.time[b]; };
	sort(ALL(X), cmp);
	fore(i,0,SZ(X)-1) X.pb(lca.lca(X[i], X[i+1]));
	sort(ALL(X), cmp), X.erase(unique(ALL(X)), X.end());
	fore(i,0,SZ(X)) rev[X[i]] = i;
	vector<ii> ret = {{{0, X[0]}}};
	fore(i,0,SZ(X)-1)
		ret.pb({rev[lca.lca(X[i], X[i+1])], X[i+1]});
	return ret;
}
