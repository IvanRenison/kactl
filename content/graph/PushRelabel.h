/**
 * Author: Simon Lindholm
 * Date: 2015-02-24
 * License: CC0
 * Source: Wikipedia, tinyKACTL
 * Description: Push-relabel using the highest label selection rule and the gap heuristic. Quite fast in practice.
 *  To obtain the actual flow, look at positive values only.
 * Time: $O(V^2\sqrt E)$
 * Status: Tested on Kattis and SPOJ, and stress-tested
 */
#pragma once


struct PushRelabel {
	struct Edge {
		ll dest, back;
		ll f, c;
	};
	vector<vector<Edge>> g;
	vi ec;
	vector<Edge*> cur;
	vector<vi> hs; vi H;
	PushRelabel(ll n) : g(n), ec(n), cur(n), hs(2*n), H(n) {}

	void addEdge(ll s, ll t, ll cap, ll rcap=0) {
		if (s == t) return;
		g[s].pb({t, SZ(g[t]), 0, cap});
		g[t].pb({s, SZ(g[s])-1, 0, rcap});
	}

	void addFlow(Edge& e, ll f) {
		Edge &back = g[e.dest][e.back];
		if (!ec[e.dest] && f) hs[H[e.dest]].pb(e.dest);
		e.f += f; e.c -= f; ec[e.dest] += f;
		back.f -= f; back.c += f; ec[back.dest] -= f;
	}
	ll calc(ll s, ll t) {
		ll v = SZ(g); H[s] = v; ec[t] = 1;
		vi co(2*v); co[0] = v-1;
		fore(i,0,v) cur[i] = g[i].data();
		for (Edge& e : g[s]) addFlow(e, e.c);

		for (ll hi = 0;;) {
			while (hs[hi].empty()) if (!hi--) return -ec[s];
			ll u = hs[hi].back(); hs[hi].pop_back();
			while (ec[u] > 0)  // discharge u
				if (cur[u] == g[u].data() + SZ(g[u])) {
					H[u] = 1e9;
					for (Edge& e : g[u]) if (e.c && H[u] > H[e.dest]+1)
						H[u] = H[e.dest]+1, cur[u] = &e;
					if (++co[H[u]], !--co[hi] && hi < v)
						fore(i,0,v) if (hi < H[i] && H[i] < v)
							--co[H[i]], H[i] = v + 1;
					hi = H[u];
				} else if (cur[u]->c && H[u] == H[cur[u]->dest]+1)
					addFlow(*cur[u], min(ec[u], cur[u]->c));
				else ++cur[u];
		}
	}
	bool leftOfMinCut(ll a) { return H[a] >= SZ(g); }
};
