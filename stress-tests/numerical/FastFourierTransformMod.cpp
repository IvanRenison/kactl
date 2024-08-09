#include "../utilities/template.h"

const ll mod = 1000000007;

#include "../../content/numerical/FastFourierTransformMod.h"

vi simpleConv(vi a, vi b) {
	if (a.empty() || b.empty()) return {};
	ll s = SZ(a) + SZ(b) - 1;
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
	vi a, b;
	fore(it,0,6000) {
		a.resize(ra() % 100);
		b.resize(ra() % 100);
		for(auto &x: a) x = ra() % mod;
		for(auto &x: b) x = ra() % mod;
		auto v1 = simpleConv(a, b);
		auto v2 = convMod<mod>(a, b);
		assert(v1 == v2);
	}
	cout<<"Tests passed!"<<endl;
}
