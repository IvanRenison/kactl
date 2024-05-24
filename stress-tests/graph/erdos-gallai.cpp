#include "../utilities/template.h"

bool valid(vi deg) {
	ll n = SZ(deg);
	sort(ALL(deg));
	reverse(ALL(deg));
	ll sum = 0;
	fore(i,0,n) sum += deg[i];
	if (sum & 1) return 0;
	fore(k,0,n) {
		ll s = 0, t = 0;
		fore(i,0,k+1)
			s += deg[i];
		fore(i,k+1,n)
			t += min(deg[i], k + 1);
		if (s > k * (k+1) + t) return 0;
	}
	return 1;
}

int main() {
	fore(N,0,7) {
		vector<ii> possibleEd;
		fore(i,0,N) fore(j,0,i) possibleEd.emplace_back(i, j);
		set<vi> valids;
		fore(bi,0,(1 << SZ(possibleEd))) {
			vi deg(N);
			fore(i,0,SZ(possibleEd)) if (bi & (1 << i)) {
				ll a, b;
				tie(a, b) = possibleEd[i];
				deg[a]++;
				deg[b]++;
			}
			assert(valid(deg));
			valids.insert(deg);
		}

		vi de(N);
		function<void(ll)> rec = [&](ll at) {
			if (at == N) {
				assert(valid(de) == valids.count(de));
			} else {
				fore(a,0,N) {
					de[at] = a;
					rec(at + 1);
				}
			}
		};
		rec(0);
	}
	cout << "Tests passed!" << endl;
}
