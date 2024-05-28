#include "../utilities/template.h"

#include "../../content/graph/MinimumVertexCover.h"
#include "../../content/graph/hopcroftKarp.h"

vi coverHK(vector<vi>& g, ll n, ll m) {
	vi match(m, -1);
	ll res = hopcroftKarp(g, match);
	vector<bool> lfound(n, true), seen(m);
	for(auto &it: match) if (it != -1) lfound[it] = false;
	vi q, cover;
	fore(i,0,n) if (lfound[i]) q.pb(i);
	while (!q.empty()) {
		ll i = q.back(); q.pop_back();
		lfound[i] = 1;
		for(auto &e: g[i]) if (!seen[e] && match[e] != -1) {
			seen[e] = true;
			q.pb(match[e]);
		}
	}
	fore(i,0,n) if (!lfound[i]) cover.pb(i);
	fore(i,0,m) if (seen[i]) cover.pb(n+i);
	assert(SZ(cover) == res);
	return cover;
}

int main() {
	fore(it,0,300000) {
		ll N = rand() % 20, M = rand() % 20;
		ll prop = rand();
		vector<vi> gr(N);
		vi left(N), right(M);
		fore(i,0,N) fore(j,0,M) if (rand() < prop) {
			gr[i].pb(j);
		}
		auto verify = [&](vi& cover) {
			for(auto &x: cover) {
				if (x < N) left[x] = 1;
				else right[x - N] = 1;
			}
			fore(i,0,N) if (!left[i]) for(auto &j:gr[i]) {
				assert(right[j]);
				/* if (!right[j]) {
					cout << N << ' ' << M << endl;
					fore(i,0,N) for(auto &j: gr[i]) cout << i << " - " << j << endl;
					cout << "yields " << SZ(cover) << endl;
					for(auto &x: cover) cout << x << endl;
					abort();
				} */
			}
		};
		vi cover1 = cover(gr, N, M);
		vi cover2 = coverHK(gr, N, M);
		assert(SZ(cover1) == SZ(cover2));
		verify(cover1);
		verify(cover2);
		// cout << '.' << endl;
	}
	cout<<"Tests passed!"<<endl;
}
