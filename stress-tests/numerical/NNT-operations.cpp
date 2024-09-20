#include "../utilities/template.h"

namespace ignoreModPow {
	#include "../../content/number-theory/ModPow.h"
}
namespace ignoreFastInverse {
	#include "../../content/number-theory/FastInverse.h"
}
ll modpow(ll a, ll e);
ll inv(ll a);
#include "../../content/numerical/NTT-operations.h"
/// content/number-theory/ModPow.h
/// START diff
/// END diff
ll modpow(ll a, ll e) {
	if (e == 0)
		return 1;
	ll x = modpow(a * a % mod, e >> 1);
	return e & 1 ? x * a % mod : x;
}
/// END content
/// content/number-theory/FastInverse.h
/// START diff
/// END diff
constexpr ll k = bit_width((unsigned long long)(mod - 2));
ll inv(ll a) {
	ll r = 1;
#pragma GCC unroll(k)
	fore(l, 0, k) {
		if ((mod - 2) >> l & 1) r = r * a % mod;
		a = a * a % mod;
	}
	return r;
}
/// END content

Poly randomPoly() {
	ll n = rand() % 20;
	Poly p(n);
	fore(i,0,n) p[i] = rand() % 10;
	while (!p.empty() && p.back() == 0) p.pop_back();
	return p;
}
Poly randomNonZeroPoly() {
	Poly p = randomPoly();
	while (p.empty()) p = randomPoly();
	return p;
}

bool PolyEq(const Poly& a, const Poly& b) {
	if (SZ(a) != SZ(b)) return false;
	fore(i,0,SZ(a)) if (a[i] != b[i]) {
		return false;
	}
	return true;
}

void testDiv() {
	Poly p = randomPoly(), q = randomNonZeroPoly();
	Poly pq = conv(p, q);
	auto [res, rem] = div(pq, q);
	assert(PolyEq(res, p));
	assert(rem.empty());
}

void testDiv2() {
	Poly p = randomPoly(), q = randomNonZeroPoly();
	auto [res, rem] = div(p, q);
	Poly pp = add(conv(res, q), rem);
	assert(PolyEq(pp, p));
	assert(rem.size() < q.size());
}


int main() {

	fore(i, 0, 1000) {
		testDiv();
		testDiv2();
	}

	cout << "Tests passed!" << endl;
}