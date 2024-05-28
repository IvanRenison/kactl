#include "../utilities/template.h"

#include "../../content/graph/LinkCutTree.h"
#include "../../content/data-structures/UnionFind.h"

int main() {
	srand(2);
	LinkCut lczero(0);
	fore(it,0,10000) {
		ll N = rand() % 20 + 1;
		LinkCut lc(N);
		UF uf(N);
		vector<ii> edges;
		fore(it2,0,1000) {
			ll v = (rand() >> 4) & 3;
			if (v == 0 && !edges.empty()) { // remove
				ll r = (rand() >> 4) % SZ(edges);
				auto [u, v] = edges[r];
				swap(edges[r], edges.back());
				edges.pop_back();
				if (rand() & 16)
					lc.cut(u, v);
				else
					lc.cut(v, u);
			} else {
				ll a = (rand() >> 4) % N;
				ll b = (rand() >> 4) % N;
				uf.e.assign(N, -1);
				for (auto [u, v] : edges) uf.join(u, v);
				bool c = uf.sameSet(a, b);
				if (!c && v != 1) {
					lc.link(a, b);
					edges.pb({a, b});
				} else {
					assert(lc.connected(a, b) == c);
				}
			}
		}
	}
	cout<<"Tests passed!"<<endl;
}
