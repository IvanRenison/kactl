#include "../utilities/template.h"
#include "../utilities/genTree.h"
#include "../../content/graph/LCA.h"
#include "../../content/graph/CompressTree.h"

struct SlowCompressTree {
	typedef vector<pair<ll, ll>> vii;

	LCA& lca;
	const vi& subset;

	SlowCompressTree(LCA& lca, const vi& subset): lca(lca), subset(subset) {}

	vii compress() {
		set<ll> nodes(subset.begin(), subset.end());
		for (size_t i = 0; i < subset.size(); ++i) {
			for (size_t j = i+1; j < subset.size(); ++j) {
				nodes.insert(lca.lca(subset[i], subset[j]));
			}
		}

		vector<ll> sortedNodes(nodes.begin(), nodes.end());
		sort(sortedNodes.begin(), sortedNodes.end(), [&](ll a, ll b) {
			return lca.time[a] < lca.time[b];
		});

		map<ll, ll> nodeToIndex;
		for (size_t i = 0; i < sortedNodes.size(); ++i) {
			nodeToIndex[sortedNodes[i]] = i;
		}

		vii tree;
		tree.emplace_back(0, sortedNodes[0]);

		for (size_t i = 1; i < sortedNodes.size(); ++i) {
			ll u = sortedNodes[i];
			ll v = sortedNodes[i - 1];
			tree.emplace_back(nodeToIndex[lca.lca(u, v)], u);
		}

		return tree;
	}
};

bool compareEdges(const vector<pair<ll, ll>>& edges1, const vector<pair<ll, ll>>& edges2) {
	set<pair<ll, ll>> set1, set2;
	for (const auto& edge : edges1) {
		set1.insert(minmax(edge.first, edge.second));
	}
	for (const auto& edge : edges2) {
		set2.insert(minmax(edge.first, edge.second));
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
