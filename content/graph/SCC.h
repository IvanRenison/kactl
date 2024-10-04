/**
 * Author: Lukas Polacek and improved by Iván Renison
 * Date: 2009-10-28
 * License: CC0
 * Source: Czech graph algorithms book, by Demel. (Tarjan's algorithm)
 * Description: Finds strongly connected components in a
 * directed graph. If vertices $u, v$ belong to the same component,
 * we can reach $u$ from $v$ and vice versa.
 * Usage: scc(adj) returns the number of components and the component of each vertex.
 * Natural order of components is reverse topological order (a component only has edges
 * to components with lower index).
 * Time: O(E + V)
 * Status: stress-tested against old implementation
 */
#pragma once

pair<ll, vi> scc(vector<vi>& g) {
	ll n = SZ(g), Time = 0, ncomps = 0;
	vi val(n), comp(n, -1), z;
	function<ll(ll)> dfs = [&](ll j) {
		ll low = val[j] = ++Time, x; z.pb(j);
		for (ll e : g[j]) if (comp[e] < 0)
			low = min(low, val[e] ?: dfs(e));
		if (low == val[j]) {
			do comp[x = z.back()] = ncomps, z.pop_back();
			while (x != j);
			ncomps++;
		}
		return val[j] = low;
	};
	fore(i, 0, n) if (comp[i] < 0) dfs(i);
	return {ncomps, comp};
}
