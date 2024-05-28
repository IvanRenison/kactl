/**
 * Author: Stanford
 * Date: Unknown
 * Source: Stanford Notebook
 * Description: Min-cost max-flow.
 *  If costs can be negative, call setpi before maxflow, but note that negative cost cycles are not supported.
 *  To obtain the actual flow, look at positive values only.
 * Status: Tested on kattis:mincostmaxflow, stress-tested against another implementation
 * Time: $O(F E \log(V))$ where F is max flow. $O(VE)$ for setpi.
 */
#pragma once

// #include <bits/extc++.h> /// include-line, keep-include

const ll INF = numeric_limits<ll>::max() / 4;

struct MCMF {
	struct edge {
		ll from, to, rev;
		ll cap, cost, flow;
	};
	ll N;
	vector<vector<edge>> ed;
	vi seen;
	vector<ll> dist, pi;
	vector<edge*> par;

	MCMF(ll N) : N(N), ed(N), seen(N), dist(N), pi(N), par(N) {}

	void addEdge(ll from, ll to, ll cap, ll cost) {
		if (from == to) return;
		ed[from].pb(edge{from, to, SZ(ed[to]), cap, cost, 0});
		ed[to].pb(edge{to, from, SZ(ed[from])-1, 0, -cost, 0});
	}

	void path(ll s) {
		fill(ALL(seen), 0);
		fill(ALL(dist), INF);
		dist[s] = 0; ll di;

		__gnu_pbds::priority_queue<ii> q;
		vector<decltype(q)::point_iterator> its(N);
		q.push({ 0, s });

		while (!q.empty()) {
			s = q.top().snd; q.pop();
			seen[s] = 1; di = dist[s] + pi[s];
			for (edge& e : ed[s]) if (!seen[e.to]) {
				ll val = di - pi[e.to] + e.cost;
				if (e.cap - e.flow > 0 && val < dist[e.to]) {
					dist[e.to] = val;
					par[e.to] = &e;
					if (its[e.to] == q.end())
						its[e.to] = q.push({ -dist[e.to], e.to });
					else
						q.modify(its[e.to], { -dist[e.to], e.to });
				}
			}
		}
		fore(i,0,N) pi[i] = min(pi[i] + dist[i], INF);
	}

	ii maxflow(ll s, ll t) {
		ll totflow = 0, totcost = 0;
		while (path(s), seen[t]) {
			ll fl = INF;
			for (edge* x = par[t]; x; x = par[x->from])
				fl = min(fl, x->cap - x->flow);

			totflow += fl;
			for (edge* x = par[t]; x; x = par[x->from]) {
				x->flow += fl;
				ed[x->to][x->rev].flow -= fl;
			}
		}
		fore(i,0,N) for(edge& e : ed[i])
			totcost += e.cost * e.flow;
		return {totflow, totcost/2};
	}

	// If some costs can be negative, call this before maxflow:
	void setpi(ll s) { // (otherwise, leave this out)
		fill(ALL(pi), INF); pi[s] = 0;
		ll it = N, ch = 1; ll v;
		while (ch-- && it--)
			fore(i,0,N) if (pi[i] != INF)
			  for (edge& e : ed[i]) if (e.cap)
				  if ((v = pi[i] + e.cost) < pi[e.to])
					  pi[e.to] = v, ch = 1;
		assert(it >= 0); // negative cost cycle
	}
};
