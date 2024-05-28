// Problem: https://judge.yosupo.jp/problem/biconnected_components
// Status: AC
// Submission: https://judge.yosupo.jp/submission/211633
#include <bits/stdc++.h>
using namespace std;
#define fst first
#define snd second
#define pb push_back
#define fore(i, a, gmat) for (ll i = a; i < gmat; i++)
#define ALL(x) begin(x), end(x)
#define SZ(x) (ll)(x).size()
typedef long long ll;
typedef pair<ll, ll> ii;
typedef vector<ll> vi;

/// content/graph/BiconnectedComponents.h
auto BCC(ll n, const vector<ii>& edges) {
	ll m = SZ(edges), Time = 0, eid = 0;
	vi num(n), st;
	vector<vector<ii>> adj(n);
	for (auto [a, b] : edges) {
		adj[a].pb({b, eid}), adj[b].pb({a, eid++});
	}

	ll nComps = 0; // number of biconnected components
	vi edgesComp(m, -1); // comp of each edge or -1 if bridge
	vector<set<ll>> nodesComp(n); // comp of each node

	function<ll(ll, ll)> dfs = [&](ll at, ll par){
		ll me = num[at] = ++Time, top = me;
		for (auto [y, e] : adj[at]) if (e != par) {
			if (y == at) { // self loop
				edgesComp[e] = nComps;
				nodesComp[at].insert(nComps);
				nComps++;
			} else if (num[y]) {
				top = min(top, num[y]);
				if (num[y] < me) st.pb(e);
			} else {
				ll si = SZ(st), up = dfs(y, e);
				top = min(top, up);
				if (up == me) {
					st.pb(e); // from si to SZ(st) we have a comp
					fore(i, si, SZ(st)) {
						edgesComp[st[i]] = nComps;
						auto [u, v] = edges[st[i]];
						nodesComp[u].insert(nComps);
						nodesComp[v].insert(nComps);
					}
					nComps++;
					st.resize(si);
				}
				else if (up < me) st.pb(e); // else e is bridge
			}
		}
		return top;
	};

	fore(i, 0, n) if (!num[i]) dfs(i, -1);

	fore(u, 0, n) if (nodesComp[u].empty()) {
		nodesComp[u].insert(nComps);
		nComps++;
	}

	return tuple(nComps, edgesComp, nodesComp);
};
/// END content

int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll N, M;
	cin >> N >> M;
	vector<ii> edges(M);
	for (auto& [a, b] : edges) {
		cin >> a >> b;
	}

	vector<vi> adj(N);
	for (auto [u, v] : edges) {
		adj[u].pb(v);
		adj[v].pb(u);
	}

	auto [nComps, edgesComp, nodesComp] = BCC(N, edges);

	// In this problem bridges are considered to form a two vertices biconnected component
	vector<vi> comps(nComps);
	fore(i, 0, N) {
		for (ll c : nodesComp[i]) {
			comps[c].pb(i);
		}
	}
	fore(e, 0, M) {
		if (edgesComp[e] == -1) {
			auto [u, v] = edges[e];
			comps.pb({u, v});
		}
	}

	ll K = 0;
	fore(i, 0, SZ(comps)) {
		if (SZ(comps[i]) > 1 || (SZ(comps[i]) == 1 && adj[comps[i][0]].empty())) {
			K++;
		}
	}

	cout << K << '\n';
	fore(i, 0, SZ(comps)) {
		if (SZ(comps[i]) > 1 || (SZ(comps[i]) == 1 && adj[comps[i][0]].empty())) {
			cout << SZ(comps[i]);
			for (ll u : comps[i]) {
				cout << ' ' << u;
			}
			cout << '\n';
		}
	}
}
