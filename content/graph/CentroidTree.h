/**
 * Author: Iván Renison
 * Date: 2024-05-24
 * License: CC0
 * Source: notebook el vasito
 * Description: Calculate the centroid tree of a tree.
 * Returns vector of fathers (-1 for root).
 * Time: $O(N)$
 * Status: Tested a bit with codeforces problem
 */
#pragma once

vi centroidTree(vector<vi>& g) {
	ll n = SZ(g);
	vector<bool> vis(n, false);
	vi fat(n), szt(n);
	function<ll(ll, ll)> calcsz = [&](ll x, ll f) {
		szt[x] = 1;
		for (ll y : g[x]) if (y != f && !vis[y])
			szt[x] += calcsz(y, x);
		return szt[x];
	};
	function<void(ll, ll, ll)> dfs = [&](ll x, ll f, ll sz) {
		if (sz < 0) sz = calcsz(x, -1);
		for (ll y : g[x]) if (!vis[y] && szt[y] * 2 >= sz) {
			szt[x] = 0;
			dfs(y, f, sz);
			return;
		}
		vis[x] = true;
		fat[x] = f;
		for (ll y : g[x]) if (!vis[y]) dfs(y, x, -1);
	};
	dfs(0, -1, -1);
	return fat;
}
