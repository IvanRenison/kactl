#include "../utilities/template.h"
#include "../utilities/genTree.h"

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
	vector<T> vals;
	vi par;

	Slow(vector<vi>& g, vector<T>& vals) : n(SZ(g)), lca(g), vals(vals), par(n, -1) {
		vi s = {0};
		while (!s.empty()) {
			ll u = s.back();
			s.pop_back();
			for (ll v : g[u]) {
				if (v != par[u]) {
					par[v] = u;
					s.pb(v);
				}
			}
		}
	}

	T query(ll u, ll v) {
		T ans = neut;
		ll p = lca.lca(u, v);
		while (u != p) {
			ans = op(ans, vals[u]);
			u = par[u];
		}
		while (v != p) {
			ans = op(ans, vals[v]);
			v = par[v];
		}
		ans = op(ans, vals[p]);
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

		vector<ll> vals(n);

		fore(i, 0, n) {
			vals[i] = rand() % 100;
		}

		PathQueries pq(adj, vals);
		Slow slow(adj, vals);

		fore(_, 0, 100) {
			ll u = rand() % n, v = rand() % n;
			ll ans_pq = pq.query(u, v);
			ll ans_slow = slow.query(u, v);
			assert(ans_pq == ans_slow);
		}
	}

	cout << "Tests passed!" << endl;
}
