#include "../utilities/template.h"

const ll mod = 5;
ll modpow(ll a, ll e) {
	if (e == 0) return 1;
	ll x = modpow(a * a % mod, e >> 1);
	return e & 1 ? x * a % mod : x;
}

#define mod dummy
#define modpow dummy2
#include "../../content/number-theory/ModPow.h"
#undef mod
#undef modpow

#include "../../content/numerical/BerlekampMassey.h"

template<class F>
void gen(vi& v, ll at, F f) {
	if (at == SZ(v)) f();
	else {
		fore(i,0,mod) {
			v[at] = i;
			gen(v, at+1, f);
		}
	}
}

int main() {
	fore(n,1,5) {
		vi start(n);
		vi coef(n), coef2;
		vi full(2*n);
		gen(start, 0, [&]() {
		gen(coef, 0, [&]() {
			fore(i,0,n) full[i] = start[i];
			fore(i,n,2*n) full[i] = 0;
			fore(i,n,2*n) fore(j,0,n) full[i] = (full[i] + coef[j] * full[i-1 - j]) % mod;
			coef2 = berlekampMassey(full);
// fore(i,0,2*n) cerr << full[i] << ' '; cerr << endl;
// fore(i,0,n) cerr << coef[i] << ' '; cerr << endl;
// fore(i,0,SZ(coef2)) cerr << coef2[i] << ' '; cerr << endl;
			if (SZ(coef2) == n) assert(coef == coef2);
// fore(i,0,n) cerr << full[i] << ' ';
			fore(i,n,2*n) {
				ll x = 0;
				fore(j,0,SZ(coef2)) x = (x + coef2[j] * full[i-1 - j]) % mod;
				// cerr << x << ' ';
				assert(x == full[i]);
			}
			// cerr << endl;
// cerr << endl;
		});
		});
	}
	cout<<"Tests passed!"<<endl;
	return 0;
}

ll main2() {
	vi v{0, 1, 1, 3, 5, 11};
	auto v2 = berlekampMassey(v);
	for(auto &x: v2) cout << x << ' ';
	cout << endl;
	return 0;
}
