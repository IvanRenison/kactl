#include "../utilities/template.h"
#include "../utilities/genTree.h"

#include "../../content/graph/BinaryLifting.h"

// Define a simple slow LCA computation for verification purpose
struct SimpleLCA {
	ll n;
	vector<vi> tree;
	vi parent, depth;
	SimpleLCA(vector<vi>& g) : n(SZ(g)), tree(g), parent(n, -1), depth(n) {
		dfs(0, -1, 0);
	}

	void dfs(ll u, ll p, ll dep) {
		parent[u] = p;
		depth[u] = dep;
		for (ll v : tree[u]) {
			if (v != p) {
				dfs(v, u, dep + 1);
			}
		}
	}

	ll lca(ll a, ll b) {
		if (depth[a] < depth[b]) swap(a, b);
		while (depth[a] > depth[b]) a = parent[a];
		while (a != b) {
			a = parent[a];
			b = parent[b];
		}
		return a;
	}
};

int main() {
	cin.tie(0)->sync_with_stdio(0);

	fore(_, 0, 1000) {
		ll n = rand() % 100 + 1;
		vector<ii> edges = n > 1 ? genRandomTree(n) : vector<ii>{};
		vector<vi> adj(n);
		vi P(n, 0);

		for (auto [u, v] : edges) {
			adj[u].pb(v);
			adj[v].pb(u);
		}

		vi parent(n, 0);
		vi depth(n, 0);
		function<void(ll, ll)> dfs = [&](ll node, ll par) {
			for (ll to : adj[node]) {
				if (to == par) continue;
				parent[to] = node;
				depth[to] = depth[node] + 1;
				dfs(to, node);
			}
		};
		dfs(0, 0);

		P = parent;
		vector<vi> jmpTbl = treeJump(P);

		SimpleLCA slow(adj);

		fore(_, 0, 100) {
			ll u = rand() % n, v = rand() % n;
			ll ans_fast = lca(jmpTbl, depth, u, v);
			ll ans_slow = slow.lca(u, v);
			assert(ans_fast == ans_slow);
		}
	}

	cout << "Tests passed!" << endl;
}
