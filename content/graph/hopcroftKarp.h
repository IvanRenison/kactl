/**
 * Author: Chen Xing
 * Date: 2009-10-13
 * License: CC0
 * Source: N/A
 * Description: Fast bipartite matching algorithm. Graph $g$ should be a list
 * of neighbors of the left partition, and $btoa$ should be a vector full of
 * -1's of the same size as the right partition. Returns the size of
 * the matching. $btoa[i]$ will be the match for vertex $i$ on the right side,
 * or $-1$ if it's not matched.
 * Usage: vi btoa(m, -1); hopcroftKarp(g, btoa);
 * Time: O(\sqrt{V}E)
 * Status: stress-tested by MinimumVertexCover, and tested on oldkattis.adkbipmatch and SPOJ:MATCHING
 */
#pragma once

bool dfs(ll a, ll L, vector<vi>& g, vi& btoa, vi& A, vi& B) {
	if (A[a] != L) return 0;
	A[a] = -1;
	for (ll b : g[a]) if (B[b] == L + 1) {
		B[b] = 0;
		if (btoa[b] == -1 || dfs(btoa[b], L + 1, g, btoa, A, B))
			return btoa[b] = a, 1;
	}
	return 0;
}

ll hopcroftKarp(vector<vi>& g, vi& btoa) {
	ll res = 0;
	vi A(g.size()), B(btoa.size()), cur, next;
	for (;;) {
		fill(ALL(A), 0);
		fill(ALL(B), 0);
		/// Find the starting nodes for BFS (i.e. layer 0).
		cur.clear();
		for (ll a : btoa) if (a != -1) A[a] = -1;
		fore(a,0,SZ(g)) if (A[a] == 0) cur.pb(a);
		/// Find all layers using bfs.
		for (ll lay = 1;; lay++) {
			bool islast = 0;
			next.clear();
			for (ll a : cur) for (ll b : g[a]) {
				if (btoa[b] == -1) {
					B[b] = lay;
					islast = 1;
				}
				else if (btoa[b] != a && !B[b]) {
					B[b] = lay;
					next.pb(btoa[b]);
				}
			}
			if (islast) break;
			if (next.empty()) return res;
			for (ll a : next) A[a] = lay;
			cur.swap(next);
		}
		/// Use DFS to scan for augmenting paths.
		fore(a,0,SZ(g))
			res += dfs(a, 0, g, btoa, A, B);
	}
}
