#include "../utilities/template.h"
#include "../utilities/genTree.h"
#include "../../content/graph/LCA.h"
#include "../../content/graph/CompressTree.h"

struct SlowCompressTree {
	typedef vector<ii> vii;

	LCA& lca;
	const vi& subset;

	SlowCompressTree(LCA& lca, const vi& subset): lca(lca), subset(subset) {}

	vii compress() {
		set<ll> nodes(ALL(subset));
		fore(i, 0, SZ(subset)) {
			fore(j, i + 1, SZ(subset)) {
				nodes.insert(lca.lca(subset[i], subset[j]));
			}
		}

		vi sortedNodes(ALL(nodes));
		sort(ALL(sortedNodes), [&](ll a, ll b) {
			return lca.time[a] < lca.time[b];
		});

		map<ll, ll> nodeToIndex;
		fore(i, 0, SZ(sortedNodes)) {
			nodeToIndex[sortedNodes[i]] = i;
		}

		vii tree;
		tree.emplace_back(0, sortedNodes[0]);

		fore(i, 1, SZ(sortedNodes)) {
			ll u = sortedNodes[i];
			ll v = sortedNodes[i - 1];
			tree.emplace_back(nodeToIndex[lca.lca(u, v)], u);
		}

		return tree;
	}
};

bool compareEdges(const vector<ii>& edges1, const vector<ii>& edges2) {
	set<ii> set1, set2;
	for (const auto& [u, v] : edges1) {
		set1.insert(minmax(u, v));
	}
	for (const auto& [u, v] : edges2) {
		set2.insert(minmax(u, v));
	}
	return set1 == set2;
}

int main() {
	cin.tie(0)->sync_with_stdio(0);

	fore(_, 0, 1000) {
		ll n = rand() % 100 + 1;
		vector<ii> edges = n > 1 ? genRandomTree(n) : vector<ii>{};
		vector<vi> adj(n);
		for (auto [u, v] : edges) {
			adj[u].pb(v), adj[v].pb(u);
		}

		LCA lca(adj);

		ll subsetSize = rand() % n + 1;
		vi subset(subsetSize);
		fore(i, 0, subsetSize) {
			subset[i] = rand() % n;
		}

		auto fastTree = compressTree(lca, subset);
		auto slowTree = SlowCompressTree(lca, subset).compress();

		assert(compareEdges(fastTree, slowTree));
	}

	cout << "Tests passed!" << endl;
}
