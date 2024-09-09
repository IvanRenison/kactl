#include "../utilities/template.h"

#include "../../content/numerical/Polynomial.h"

Poly mul(const Poly& p, const Poly& q) {
	if (p.empty() || q.empty()) return {};
	Poly res(SZ(p) + SZ(q) - 1);
	fore(i,0,SZ(p)) fore(j,0,SZ(q))
		res[i+j] += p[i] * q[j];
	return res;
}
Poly add(const Poly& p, const Poly& q) {
	Poly res(max(SZ(p), SZ(q)));
	fore(i,0,SZ(p)) res[i] += p[i];
	fore(i,0,SZ(q)) res[i] += q[i];
	while (!res.empty() && abs(res.back()) < eps) res.pop_back();
	return res;
}

Poly randomPoly() {
	ll n = rand() % 20;
	Poly p(n);
	fore(i,0,n) p[i] = rand() % 10 - 5;
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
	fore(i,0,SZ(a)) if (abs(a[i] - b[i]) > eps) return false;
	return true;
}

void testDiv() {
	Poly p = randomPoly(), q = randomNonZeroPoly();
	Poly pq = mul(p, q);
	auto [res, rem] = div(pq, q);
	assert(PolyEq(res, p));
	assert(rem.empty());
}

void testDiv2() {
	Poly p = randomPoly(), q = randomNonZeroPoly();
	auto [res, rem] = div(p, q);
	Poly pp = add(mul(res, q), rem);
	assert(PolyEq(pp, p));
	assert(rem.size() < q.size());
}

void testDivroot() {
	Poly p = randomNonZeroPoly();
	double x0 = rand() % 10 - 5;
	Poly q = {-x0, 1};
	Poly pq = mul(p, q);
	auto [res, rem] = divroot(pq, x0);
	assert(PolyEq(res, p));
	assert(abs(rem) < eps);
}

int main() {

	fore(i, 0, 1000) {
		testDiv();
		testDiv2();
		testDivroot();
	}

	cout << "Tests passed!" << endl;
}
