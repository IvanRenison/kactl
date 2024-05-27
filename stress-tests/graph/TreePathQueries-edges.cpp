#include "../utilities/template.h"
#include "../utilities/genTree.h"

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
//PathQueries(const vector<vi>& g, vector<T>& vals) // NODES
//	: n(SZ(g)), K(64 - __builtin_clzll(n)), anc(K, vi(n, -1)),
//		part(K, vector<T>(n, neut)), depth(n) {
//	part[0] = vals;
	PathQueries(vector<vector<pair<ll, T>>> &g_) // EDGES
		: n(SZ(g_)), K(64 - __builtin_clzll(n)), anc(K, vi(n, -1)),
			part(K, vector<T>(n, neut)), depth(n) {
		vector<vi> g(n);
		fore(u, 0, n) for (auto [v, data] : g_[u]) {
			g[u].pb(v);
		}
		vi s = {0};
		while (!s.empty()) {
			ll u = s.back();
			s.pop_back();
			for (ll v : g[u]) if (v != anc[0][u]) {
				anc[0][v] = u, depth[v] = depth[u] + 1, s.pb(v);
			}
		}
		fore(u, 0, n) for (auto [v, data] : g_[u]) { // EDGES
			part[0][depth[u] > depth[v] ? u : v] = data;
		}
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
//	if (u == v) return f(ans, part[0][u]); // NODES
		if (u == v) return ans; // EDGES
		for (ll k = K; k--;) if (anc[k][u] != anc[k][v]) {
			ans = f(ans, f(part[k][u], part[k][v]));
			u = anc[k][u], v = anc[k][v];
		}
		ans = f(ans, f(part[0][u], part[0][v]));
//	return f(ans, part[0][anc[0][u]]); // NODES
		return ans; // EDGES
	}
};

#include "../../content/graph/LCA.h"

struct Slow {
	typedef ll T;
	constexpr static T neut = LONG_LONG_MIN;
	T f(const T& a, const T& b) {
		return max(a, b);
	}

	ll n;
	LCA lca;
	vector<pair<ll, T>> par;

	Slow(vector<vi>& g, vector<vector<pair<ll, T>>>& g_vals) : n(SZ(g)), lca(g), par(n, {-1, neut}) {
		vi s = {0};
		while (!s.empty()) {
			ll u = s.back();
			s.pop_back();
			for (auto [v, x] : g_vals[u]) {
				if (v != par[u].fst) {
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
			ans = f(ans, par[u].snd);
			u = par[u].fst;
		}
		while (v != p) {
			ans = f(ans, par[v].snd);
			v = par[v].fst;
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

		vector<vector<ii>> g_vals(n);
		fore(i, 0, n - 1) {
			ll u = edges[i].fst, v = edges[i].snd;
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
