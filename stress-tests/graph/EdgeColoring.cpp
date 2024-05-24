#include <random>
#include "../utilities/template.h"
#include "../utilities/genGraph.h"
#include "../utilities/random.h"

#include "../../content/graph/EdgeColoring.h"

void test(ll n, const vector<ii>& ed) {
	vi deg(n);
	for (ii e : ed) ++deg[e.fst], ++deg[e.snd];
	ll maxdeg = n == 0 ? 0 : *max_element(ALL(deg));
	vi cols = edgeColoring(n, ed);
	assert(SZ(cols) == SZ(ed));
	vector<vector<bool>> usedCols(n, vector<bool>(maxdeg+1));
	fore(i,0,SZ(cols)) {
		ll col = cols[i];
		assert(cols[i] <= maxdeg);
		for (ll x : {ed[i].fst, ed[i].snd}) {
			assert(!usedCols[x][col]);
			usedCols[x][col] = 1;
		}
	}
}

void testCorrect() {
	fore(n,0,7) {
		fore(edbits,0,(1 << (n*(n-1)/2))) {
			vector<ii> ed;
			ll it = 0;
			fore(i,0,n) fore(j,i+1,n) {
				if (edbits & 1 << (it++)) {
					ed.pb({i, j});
				}
			}
			if (n <= 4 || n + SZ(ed) <= 9) {
				// test all k!*2^k input orders
				sort(ALL(ed));
				if (n != 0) do {
					fore(bi,0,(1 << SZ(ed))) {
						if (bi) {
							ll ind = __builtin_ctzll(bi);
							swap(ed[ind].fst, ed[ind].snd);
						}
						test(n, ed);
					}
				} while (next_permutation(ALL(ed)));
			} else {
				ll its = n == 5 ? 10 : 5;
				fore(it,0,its) {
					shuffle_vec(ed);
					for (auto& e : ed) if (randBool()) swap(e.fst, e.snd);
					test(n, ed);
				}
			}
		}
	}
	fore(n,10,30) fore(it,0,200) {
		ll m = randIncl(n * (n-1) / 2);
		vector<ii> ed = randomSimpleGraphAsEdgeList(n, m);
		test(n, ed);
	}
	for (ll n = 1; n <= 1000000; n *= 2) {
		ll m = (ll) min(1000000LL / n, (ll)n * (n-1) / 2);
		auto ed = randomSimpleGraphAsEdgeList(n, m);
		test(n, ed);
	}
	cout << "Tests passed!" << endl;
}

void testPerfRandom() {
	fore(i,0,100) {
		ll n = 1000;
		ll m = 20000;
		auto ed = randomSimpleGraphAsEdgeList(n, m);
		edgeColoring(n, ed);
	}
}

void testPerfRegular() {
	ll n = 3000;
	ll k = 30;
	// m = 45000
	vector<ii> ed = randomRegularGraphAsEdgeList(n, k);
	fore(i,0,100) edgeColoring(n, ed);
}

int main(int argc, char** argv) {
	srand(2);
	string arg = argc == 1 ? "" : argv[1];
	if (arg == "random") testPerfRandom();
	else if (arg == "regular") testPerfRegular();
	else {
		assert(argc == 1);
		testCorrect();
	}
}
