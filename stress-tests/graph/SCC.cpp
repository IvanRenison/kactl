#include "../utilities/template.h"

#include "../../content/graph/SCC.h"

namespace old {
vi orig, low, comp, z;
ll no_vertices, no_components;
template<class G> void dfs(ll j, G &g) {
	low[j] = orig[j] = no_vertices++;
	comp[j] = -2; z.pb(j);
	for(auto &e:g[j])
		if (comp[e] == -1) {
			dfs(e, g);
			low[j] = min(low[j], low[e]);
		}
		else if (comp[e] == -2)
			low[j] = min(low[j], orig[e]);

	if (orig[j] == low[j]) {
		for (;;) {
			ll x = z.back(); z.pop_back();
			comp[x] = no_components;
			if (x == j) break;
		}
		no_components++;
	}
}
template<class G> vi scc(G &g) {
	ll n = SZ(g);
	orig.assign(n, 0); low = orig;
	no_vertices = no_components = 0;
	comp.assign(n, -1);
	fore(i,0,n) if (comp[i] == -1) dfs(i, g);
	return comp;
}
}

void test(vector<vi>& g) {
	auto [ncomps, comp] = scc(g);
	vi comp2 = old::scc(g);
	assert(comp == comp2);
}

int main() {
	fore(N, 0, 5) {
		fore(bits, 0 , 1 << (N*N)) {
			vector<vi> mat(N, vi(N)), adj(N);
			fore(i,0,N) fore(j,0,N)
				mat[i][j] = bits & 1 << (i*N+j);

			fore(i,0,N) fore(j,0,N) if (mat[i][j]) adj[i].pb(j);

			test(adj);
		}
	}

	fore(_, 0, 1000) {
		ll N = rand() % 10 + 5;
		vector<vi> adj(N);
		fore(i,0,N) fore(j,0,N) if (i != j && rand() % 2) adj[i].pb(j);
		test(adj);
	}

	cout << "Tests passed!" << endl;
}
