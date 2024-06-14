#include "../utilities/template.h"

#include "../../content/graph/MaximalCliques.h"

template<class F>
void fastCliques(vector<B>& eds, F f) {
	B R{}, P = ~B(), X{};
	vi deg(SZ(eds));
	fore(i,0,SZ(eds)) deg[i] = SZ(eds[i]);
	fore(j,0,SZ(eds)) {
		ll i = (ll)(min_element(ALL(deg)) - deg.begin());
		R[i] = 1;
		rec(eds, R, P & eds[i], X & eds[i], f);
		R[i] = P[i] = 0; X[i] = 1;
		fore(k,0,SZ(eds)) if (eds[i][k]) deg[k]--;
		deg[i] = 1000000;
	}
}

ll main1() {
	fore(n,1,11) fore(m,0,200) {
		vector<B> ed(n);
		fore(i,0,m) {
			ll a = rand() % n, b = rand() % n;
			if (a == b) continue;
			ed[a][b] = 1;
			ed[b][a] = 1;
		}
		unordered_set<B> cl;
		ll co = 0;
		cliques(ed, [&](B x) {
			co++;
			cl.insert(x);
		});
		assert(SZ(cl) == co); // no duplicates
		auto isClique = [&](B c) {
			fore(i,0,n) if (c[i])
			fore(j,i+1,n) if (c[j]) {
				if (!ed[i][j]) return false; // not a clique
			}
			fore(i,0,n) if (!c[i]) {
				bool all = true;
				fore(j,0,n) if (c[j]) all &= ed[i][j];
				if (all) return false; // not maximal
			}
			return true;
		};
		for(auto &c: cl) {
			assert(isClique(c));
		}

		ll realCo = 0;
		fore(bi,0,(1 << n)) {
			B c{};
			fore(i,0,n) c[i] = !!(bi & (1 << i));
			if (isClique(c)) realCo++;
		}
		assert(co == realCo);
	}
	cout<<"Tests passed!"<<endl;
	return 0;
}

ll main2() {
	fore(it,0,20) {
		const ll n = 128, m = 4000;
		vector<B> ed(n);
		fore(i,0,m) {
			ll a = rand() % n, b = rand() % n;
			if (a == b) continue;
			ed[a][b] = 1;
			ed[b][a] = 1;
		}
		ll co = 0, sum = 0;
		cliques(ed, [&](B x) { co++; sum += (ll)x.count(); });
		cout << co << ' ' << (double)sum / co << endl;
	}
	return 0;
}

#ifndef target
#define target main1
#endif
int main() { target(); }
