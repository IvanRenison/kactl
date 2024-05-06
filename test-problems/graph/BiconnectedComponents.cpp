// Problem: https://judge.yosupo.jp/problem/biconnected_components
// Status: AC
// Submission: https://judge.yosupo.jp/submission/207135
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
					st.push_back(e);//from si to SZ(st) we have a comp
					fore(i, si, SZ(st)) {
						ans.edgesComp[st[i]] = ans.nComps;
						auto [u, v] = edges[st[i]];
						ans.nodesComp[u].insert(ans.nComps);
						ans.nodesComp[v].insert(ans.nComps);
					}
					ans.nComps++;
					st.resize(si);
				}
				else if (up < me) st.push_back(e); // else e isa bridge
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
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	BCC_ans ans = BCC(N, edges);

	// In this problem bridges are considered to form a two vertices biconnected component
	vector<vi> comps(ans.nComps);
	fore(i, 0, N) {
		for (ll c : ans.nodesComp[i]) {
			comps[c].push_back(i);
		}
	}
	fore(e, 0, M) {
		if (ans.edgesComp[e] == -1) {
			auto [u, v] = edges[e];
			comps.push_back({u, v});
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
