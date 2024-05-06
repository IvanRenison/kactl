/**
 * Author: Simon Lindholm
 * Date: 2017-04-17
 * License: CC0
 * Source: folklore
 * Description: Finds all biconnected components in an undirected graph, and
 *  runs a callback for the edges in each. In a biconnected component there
 *  are at least two distinct paths between any two nodes. Note that a node can
 *  be in several components. An edge which is not in a component is a bridge,
 *  i.e., not part of any cycle.
 * Time: O(E + V)
 * Status: untested
 */
#pragma once

struct BCC_ans {
	ll nComps; // number of biconnected components
	vi edgesComp; // component of each edge or -1 if bridge
	vector<set<ll>> nodesComp; // component of each node
};

BCC_ans BCC(ll n, const vector<ii>& edges) {
	ll m = SZ(edges), Time = 0, eid = 0;
	vi num(n), st;
	vector<vector<ii>> adj(n);
	for (auto [a, b] : edges) {
		adj[a].emplace_back(b, eid), adj[b].emplace_back(a, eid++);
	}

	BCC_ans ans = {0, vi(m, -1), vector<set<ll>>(n)};

	function<ll(ll, ll)> dfs = [&](ll at, ll par){
		ll me = num[at] = ++Time, top = me;
		for (auto [y, e] : adj[at]) if (e != par) {
			if (y == at) { // self loop
				ans.edgesComp[e] = ans.nComps;
				ans.nodesComp[at].insert(ans.nComps);
				ans.nComps++;
			} else if (num[y]) {
				top = min(top, num[y]);
				if (num[y] < me) st.push_back(e);
			} else {
				ll si = SZ(st), up = dfs(y, e);
				top = min(top, up);
				if (up == me) {
					st.push_back(e); // from si to SZ(st) we have a comp
					fore(i, si, SZ(st)) {
						ans.edgesComp[st[i]] = ans.nComps;
						auto [u, v] = edges[st[i]];
						ans.nodesComp[u].insert(ans.nComps);
						ans.nodesComp[v].insert(ans.nComps);
					}
					ans.nComps++;
					st.resize(si);
				}
				else if (up < me) st.push_back(e); // else e is bridge
			}
		}
		return top;
	};

	fore(i, 0, n) if (!num[i]) dfs(i, -1);

	fore(u, 0, n) if (ans.nodesComp[u].empty()) {
		ans.nodesComp[u].insert(ans.nComps);
		ans.nComps++;
	}

	return ans;
};
