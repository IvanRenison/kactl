#include "../utilities/template.h"

ll det(vector<vi>& a) {
	ll n = SZ(a); ll ans = 1;
	fore(i,0,n) {
		fore(j,i+1,n) {
			while (a[j][i] != 0) { // gcd step
				ll t = a[i][i] / a[j][i];
				if (t) fore(k,i,n)
					a[i][k] = (a[i][k] - a[j][k] * t);
				swap(a[i], a[j]);
				ans *= -1;
			}
		}
		ans = ans * a[i][i];
		if (!ans) return 0;
	}
	return ans;
}

ll rec(vector<vi>& ed, ll active, ll finished) {
	ll N = SZ(ed);
	if (finished == (1 << N) - 1) return 1;
	ll chosen = -1;
	fore(i,0,N) if (!(finished & (1 << i)) && (active & (1 << i))) {
		chosen = i;
		break;
	}
	if (chosen == -1) return 0; // missing nodes from tree
	ll si = SZ(ed[chosen]);
	ll res = 0;
	fore(bi,0,(1 << si)) {
		ll edto = 0;
		fore(i,0,si) {
			if (bi & (1 << i)) {
				if (edto & (1 << ed[chosen][i])) goto fail;
				edto |= 1 << ed[chosen][i];
			}
		}
		if (active & edto) continue;
		res += rec(ed, active | edto, finished | (1 << chosen));
fail:;
	}
	return res;
}

ll countTrees(vector<vi>& ed, ll root) {
	return rec(ed, 1 << root, 0);
}

int main() {
	fore(it,0,100) {
		ll N = rand() % 7 + 1;
		ll M = rand() % 10;
		ll root = rand() % N;
		vector<vi> ed(N);
		vector<vi> mat(N, vi(N));
		fore(i,0,M) {
			ll a = rand() % N;
			ll b = rand() % N;
			ed[a].pb(b);
			mat[b][b]++;
			mat[a][b]--;
		}
		ll numTrees = countTrees(ed, root);

		mat.erase(mat.begin() + root);
		fore(i,0,N-1) mat[i].erase(mat[i].begin() + root);

		ll numTrees2 = det(mat);
		assert(numTrees == numTrees2);
	}
	cout << "Tests passed!" << endl;
}
