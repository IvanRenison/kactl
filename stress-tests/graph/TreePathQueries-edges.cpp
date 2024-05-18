#include "../utilities/template.h"
#include "../utilities/genTree.h"

#define EDGES
#include "../../content/graph/TreePathQueries.h"

#include "../../content/graph/LCA.h"

struct Slow {
	typedef ll T;
	constexpr static T neut = LONG_LONG_MIN;
	T op(const T& a, const T& b) {
		return max(a, b);
	} // associative and commutative

	ll n;
	LCA lca;
	vector<pair<ll, T>> par;

	Slow(vector<vi>& g, vector<vector<pair<ll, T>>>& g_vals) : n(SZ(g)), lca(g), par(n, {-1, neut}) {
		vi s = {0};
		while (!s.empty()) {
			ll u = s.back();
			s.pop_back();
			for (auto [v, x] : g_vals[u]) {
				if (v != par[u].first) {
					par[v] = {u, x};
					s.pb(v);
				}
			}
		}
	}

	T query(ll u, ll v) {
		T ans = neut;
		ll p = lca.lca(u, v);
		while (u != p) {
			ans = op(ans, par[u].second);
			u = par[u].first;
		}
		while (v != p) {
			ans = op(ans, par[v].second);
			v = par[v].first;
		}
		return ans;
	}
};

int main() {
	cin.tie(0)->sync_with_stdio(0);

	fore(_, 0, 1000) {
		ll n = rand() % 100 + 1;
		vector<ii> edges = n > 1 ? genRandomTree(n) : vector<ii>{};
		vector<vi> adj(n);
		for (auto [u, v] : edges) {
			adj[u].pb(v), adj[v].pb(u);
		}

		vector<ll> vals(n - 1);

		fore(i, 0, n - 1) {
			vals[i] = rand() % 100;
		}

		vector<vector<pair<ll, ll>>> g_vals(n);
		fore(i, 0, n - 1) {
			ll u = edges[i].first, v = edges[i].second;
			g_vals[u].pb({v, vals[i]});
			g_vals[v].pb({u, vals[i]});
		}

		PathQueries pq(g_vals);
		Slow slow(adj, g_vals);

		fore(_, 0, 100) {
			ll u = rand() % n, v = rand() % n;
			ll ans_pq = pq.query(u, v);
			ll ans_slow = slow.query(u, v);
			assert(ans_pq == ans_slow);
		}
	}

	cout << "Tests passed!" << endl;
}