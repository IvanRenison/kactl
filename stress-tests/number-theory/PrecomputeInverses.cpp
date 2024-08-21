#include "../utilities/template.h"

ll mod, LIM = 1000;
array<ll, LIM> inv;
void initInv() {
	inv[1] = 1;
	fore(i,2,LIM) inv[i] = mod - (mod / i) * inv[mod % i] % mod;
}

ll modpow(ll a, ll e, ll mod) {
	if (e == 0) return 1;
	ll x = modpow(a * a % mod, e >> 1, mod);
	return e & 1 ? x * a % mod : x;
}
bool isPrime(ll x) {
	if (x <= 1) return false;
	for (ll i = 2; i*i <= x; ++i) {
		if (x % i == 0) return false;
	}
	return true;
}
int main() {
	fore(it,1,1000) {
		mod = it;
		if (!isPrime(mod)) continue;
		initInv();
		for (ll i=1; i<it; i++){
			assert(inv[i] == modpow(i, mod-2, mod));
		}
	}
	cout<<"Tests pass!"<<endl;
}
