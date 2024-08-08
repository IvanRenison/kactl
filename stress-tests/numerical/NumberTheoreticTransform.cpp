#include "../utilities/template.h"

namespace ignore {
#include "../../content/number-theory/ModPow.h"
}
ll modpow(ll a, ll e);
#include "../../content/numerical/NumberTheoreticTransform.h"
ll modpow(ll a, ll e) {
	if (e == 0)
		return 1;
	ll x = modpow(a * a % mod, e >> 1);
	return e & 1 ? x * a % mod : x;
}

vi simpleConv(vi a, vi b) {
	ll s = SZ(a) + SZ(b) - 1;
	if (a.empty() || b.empty()) return {};
	vi c(s);
	fore(i,0,SZ(a)) fore(j,0,SZ(b))
		c[i+j] = (c[i+j] + (ll)a[i] * b[j]) % mod;
	for(auto &x: c) if (x < 0) x += mod;
	return c;
}

ll ra() {
	static unsigned X;
	X *= 123671231;
	X += 1238713;
	X ^= 1237618;
	return (X >> 1);
}

int main() {
	ll res = 0, res2 = 0;
	ll ind = 0, ind2 = 0;
	vi a, b;
	fore(it,0,6000) {
		a.resize(ra() % 10);
		b.resize(ra() % 10);
		for(auto &x: a) x = (ra() % 100 - 50+mod)%mod;
		for(auto &x: b) x = (ra() % 100 - 50+mod)%mod;
		for(auto &x: simpleConv(a, b)) res += (ll)x * ind++ % mod;
		for(auto &x: conv(a, b)) res2 += (ll)x * ind2++ % mod;
		a.resize(16);
			vi a2 = a;
			ntt(a2);
			fore(k, 0, SZ(a2)) {
				ll sum = 0;
				fore(x, 0, SZ(a2)) { sum = (sum + a[x] * modpow(root, k * x * (mod - 1) / SZ(a))) % mod; }
				assert(sum == a2[k]);
			}
	}
	assert(res==res2);
	cout<<"Tests passed!"<<endl;
}
