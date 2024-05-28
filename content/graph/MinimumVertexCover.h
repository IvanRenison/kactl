/**
 * Author: Johan Sannemo, Simon Lindholm
 * Date: 2016-12-15
 * License: CC0
 * Description: Finds a minimum vertex cover in a bipartite graph.
 *  The size is the same as the size of a maximum matching, and
 *  the complement is a maximum independent set.
 * Status: stress-tested
 */
#pragma once

#include "DFSMatching.h"

vi cover(vector<vi>& g, ll n, ll m) {
	vi match(m, -1);
	ll res = dfsMatching(g, match);
	vector<bool> lfound(n, true), seen(m);
	for (ll it : match) if (it != -1) lfound[it] = false;
	vi q, cover;
	fore(i,0,n) if (lfound[i]) q.pb(i);
	while (!q.empty()) {
		ll i = q.back(); q.pop_back();
		lfound[i] = 1;
		for (ll e : g[i]) if (!seen[e] && match[e] != -1) {
			seen[e] = true;
			q.pb(match[e]);
		}
	}
	fore(i,0,n) if (!lfound[i]) cover.pb(i);
	fore(i,0,m) if (seen[i]) cover.pb(n+i);
	assert(SZ(cover) == res);
	return cover;
}
