#include "../utilities/template.h"

#include "../../content/geometry/ConvexHull.h"
#include "../utilities/bench.h"

namespace old {
pair<vi, vi> ulHull(const vector<P>& S) {
	vi Q(SZ(S)), U, L;
	iota(ALL(Q), 0);
	sort(ALL(Q), [&S](ll a, ll b){ return S[a] < S[b]; });
	for(auto &it: Q) {
#define ADDP(C, cmp) while (SZ(C) > 1 && S[C[SZ(C)-2]].cross(\
	S[it], S[C.back()]) cmp 0) C.pop_back(); C.pb(it);
		ADDP(U, <=); ADDP(L, >=);
	}
	return {U, L};
}

vi convexHull(const vector<P>& S) {
	vi u, l; tie(u, l) = ulHull(S);
	if (SZ(S) <= 1) return u;
	if (S[u[0]] == S[u[1]]) return {0};
	l.insert(l.end(), u.rbegin()+1, u.rend()-1);
	return l;
}
}

int main() {
	const ll SZ = 1e2;
	fore(t,0,100000) {
		const ll GRID=1e3;
		vector<P> pts(SZ);
		fore(i,0,SZ) pts[i] = P(rand()%GRID, rand()%GRID);
		auto res = convexHull(pts);
		auto res2 = old::convexHull(pts);
		assert(SZ(res) == SZ(res2));
		fore(i,0,SZ(res2)) {
			assert(pts[res2[i]] == res[i]);
		}
	}
	cout << "Tests passed!" << endl;
}
