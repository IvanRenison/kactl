/**
 * Author: Iván Renison
 * Date: 2024-09-08
 * Source: Original from kactl and notebook el vasito
 * Description: Some operations with polynomials. Everything except \texttt{div} works
 * also with integers. For a faster version of \texttt{mul}, see FFT.
 * Status: stress-tested a bit
 */
#pragma once

const double eps = 1e-9;
typedef vector<double> Poly;

double eval(const Poly& p, double x) { // O(n)
	double val = 0;
	for (ll i = SZ(p); i--;) (val *= x) += p[i];
	return val;
}
Poly derivate(const Poly& p) { // O(n)
	Poly res(SZ(p)-1);
	fore(i, 1, SZ(p)) res[i-1] = i*p[i];
	return res;
}
Poly add(const Poly& p, const Poly& q) { // O(n)
	Poly res(max(SZ(p), SZ(q)));
	fore(i,0,SZ(p)) res[i] += p[i];
	fore(i,0,SZ(q)) res[i] += q[i];
	while (!res.empty() && abs(res.back()) < eps) res.pop_back();
	return res;
}
Poly mul(const Poly& p, const Poly& q) { // O(n^2)
	if (p.empty() || q.empty()) return {};
	Poly res(SZ(p) + SZ(q) - 1);
	fore(i,0,SZ(p)) fore(j,0,SZ(q))
		res[i+j] += p[i] * q[j];
	return res;
}
pair<Poly, double> divSmall(const Poly& p, double x0) { // O(n)
	ll n = SZ(p)-1;     // Divide p by (x-x0), returns {res, rem}
	if (n < 0) return {{}, 0};
	if (n == 0) return {{}, p[0]};
	Poly res(n);
	res[n-1] = p[n];
	for (ll i = n - 1; i--;) res[i] = p[i+1] + x0 * res[i+1];
	return {res, p[0] + x0 * res[0]};
}
pair<Poly, Poly> div(Poly p, const Poly& q) { // O(n^2)
	if (SZ(p) < SZ(q)) return {{}, p};  // returns {res, rem}
	ll n = SZ(p) - SZ(q) + 1;
	Poly res(n);
	for (ll i = n; i--;) {
		res[i] = p.back() / q.back();
		fore(j, 0, SZ(q)) p[j + i] -= res[i] * q[j];
		p.pop_back();
	}
	while (!p.empty() && abs(p.back()) < eps) p.pop_back();
	return {res, p};
}
