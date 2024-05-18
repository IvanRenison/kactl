/**
 * Author: Iván Renison
 * Date: 2024-05-18
 * License: CC0
 * Source: Folklore
 * Description: Data structure for computing queries on paths in a tree.
 * Time: construction $O(N \log N)$, queries $O(\log N)$
 * Status: stress-tested a bit
 */
#pragma once

struct PathQueries {
	typedef ll T;
	constexpr static T neut = LONG_LONG_MIN;
	T f(const T& a, const T& b) {
		return max(a, b);
	} // (any associative and commutative fn)

	ll n, K;
	vector<vi> anc;
	vector<vector<T>> part;
	vi depth;
	PathQueries(const vector<vi>& g, vector<T>& vals) // NODES
		: n(SZ(g)), K(64 - __builtin_clzll(n)), anc(K, vi(n, -1)),
			part(K, vector<T>(n, neut)), depth(n) {
		part[0] = vals;
//PathQueries(vector<vector<pair<ll, T>>> &g_) // EDGES
//	: n(SZ(g_)), K(64 - __builtin_clzll(n)), anc(K, vi(n, -1)),
//		part(K, vector<T>(n, neut)), depth(n) {
//	vector<vi> g(n);
//	fore(u, 0, n) for (auto [v, data] : g_[u]) {
//		g[u].pb(v);
//	}
		vi s = {0};
		while (!s.empty()) {
			ll u = s.back();
			s.pop_back();
			for (ll v : g[u]) if (v != anc[0][u]) {
				anc[0][v] = u, depth[v] = depth[u] + 1, s.push_back(v);
			}
		}
//	fore(u, 0, n) for (auto [v, data] : g_[u]) { // EDGES
//		part[0][depth[u] > depth[v] ? u : v] = data;
//	}
		fore(k, 0, K - 1) fore(v, 0, n) {
			if (anc[k][v] != -1) {
				anc[k + 1][v] = anc[k][anc[k][v]];
				part[k + 1][v] = f(part[k][v], part[k][anc[k][v]]);
			}
		}
	}
	T query(ll u, ll v) {
		if (depth[u] < depth[v]) swap(u, v);
		T ans = neut;
		fore(k, 0, K) if ((depth[u] - depth[v]) & (1 << k))
			ans = f(ans, part[k][u]), u = anc[k][u];
		if (u == v) return f(ans, part[0][u]); // NODES
//	if (u == v) return ans; // EDGES
		for (ll k = K; k--;) if (anc[k][u] != anc[k][v]) {
			ans = f(ans, f(part[k][u], part[k][v]));
			u = anc[k][u], v = anc[k][v];
		}
		ans = f(ans, f(part[0][u], part[0][v]));
		return f(ans, part[0][anc[0][u]]); // NODES
//	return ans; // EDGES
	}
};
