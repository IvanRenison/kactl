/**
 * Author: Chen Xing
 * Date: 2009-10-13
 * License: CC0
 * Source: N/A
 * Description: Flow algorithm with guaranteed complexity $O(VE^2)$. To get edge flow values, compare
 * capacities before and after, and take the positive values only.
 * Status: stress-tested
 */
#pragma once

template<class T> T edmondsKarp(vector<unordered_map<ll, T>>&
		graph, ll source, ll sink) {
	assert(source != sink);
	T flow = 0;
	vi par(SZ(graph)), q = par;

	for (;;) {
		fill(ALL(par), -1);
		par[source] = 0;
		ll ptr = 1;
		q[0] = source;

		for (ll i = 0; i < ptr; i++) {
			ll x = q[i];
			for (auto e : graph[x]) {
				if (par[e.fst] == -1 && e.snd > 0) {
					par[e.fst] = x;
					q[ptr++] = e.fst;
					if (e.fst == sink) goto out;
				}
			}
		}
		return flow;
out:
		T inc = numeric_limits<T>::max();
		for (ll y = sink; y != source; y = par[y])
			inc = min(inc, graph[par[y]][y]);

		flow += inc;
		for (ll y = sink; y != source; y = par[y]) {
			ll p = par[y];
			if ((graph[p][y] -= inc) <= 0) graph[p].erase(y);
			graph[y][p] += inc;
		}
	}
}
