/**
 * Author: Unknown, Johan Sannemo
 * Date: 2009-04-17
 * Source: tinyKACTL
 * Description: Old min-cost max-flow. Slow, but probably supports negative costs and double edges. Returns (flow, cost).
 * Status: Tested
 */
#pragma once

typedef ll Flow;
Flow inf = 1ll << 56;

struct FlowEdge {
	ll dest, back;
	Flow c, f, cost;
	Flow r() { return c - f; }
	FlowEdge(ll d, ll b, Flow c, Flow cost = 0)
		: dest(d), back(b), c(c), f(0), cost(cost) {}
};

template<class G>
void flow_add_edge(G& g, ll s, ll t,
		Flow c, Flow cost = 0) {
	assert(s != t);
	g[s].pb(FlowEdge(t, SZ(g[t]), c, cost));
	g[t].pb(FlowEdge(s, SZ(g[s]) - 1, 0, cost));
}

template<class G>
pair<Flow, Flow> aug(G &g, ll s, ll t) {
	ll n = SZ(g);
	vi mark(n, -1);
	vector<Flow> mindist(n, inf);
	bool changed = true; mindist[s] = 0;
	for (ll i = 1; !(changed = !changed); ++i)
		for (ll v = 0; v < n; ++v) if (mindist[v] != inf)
			for(auto &e: g[v]) {
				Flow dist = mindist[v] + (e.f<0 ? -e.cost : e.cost);
				if (e.r() > 0 && dist < mindist[e.dest]) {
					if (i >= n) assert(0);// negative cycle! shouldn't be
					mindist[e.dest] = dist;
					mark[e.dest] = e.back;
					changed = true;
				}
			}
	if (mark[t] < 0) return make_pair(0, 0);
	Flow inc = inf;
	FlowEdge* e; ll v = t;
	while (v != s) {
		e = &g[v][mark[v]];
		v = e->dest;
		inc = min(inc, g[v][e->back].r());
	}
	v = t;
	while (v != s)
		e = &g[v][mark[v]], e->f -= inc,
		  v = e->dest, g[v][e->back].f += inc;
	return make_pair(inc, inc * mindist[t]);
}

template<class G>
pair<Flow, Flow> min_cost_max_flow(G& graph, ll s, ll t) {
	pair<Flow, Flow> flow, inc;
	while ((inc = aug(graph, s, t)).fst) {
		flow.fst += inc.fst;
		flow.snd += inc.snd;
	}
	return flow;
}
