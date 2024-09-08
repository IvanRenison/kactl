/**
 * Author: Iván Renison
 * Date: 2024-09-08
 * Source: Original from kactl and notebook el vasito
 * Description: Some operations with polynomials. Everything except \texttt{div} works
 * also with integers.
 * Time: All O(n) except for \texttt{div} witch is O(n^2)
 * Status: untested
 */
#pragma once

const double eps = 1e-9;
typedef vector<double> Poly;

double eval(const Poly& p, double x) {
	double val = 0;
	for (ll i = SZ(p); i--;) (val *= x) += p[i];
	return val;
}
Poly derivate(const Poly& p) {
	Poly res(SZ(p)-1);
	fore(i, 1, SZ(p)) res[i-1] = i*p[i];
	return res;
}
pair<Poly, double> divroot(const Poly& p, double x0) {
	ll n = SZ(p) - 1;            // Divide p by (x-x0)
	Poly res(n);
	res[n-1] = p[n];
	for (ll i = n - 1; i--; ) res[i] = p[i+1] + x0 * res[i+1];
	return {res, p[0] + x0 * res[0]};
}
pair<Poly, Poly> div(Poly p, const Poly& q) {
	ll n= SZ(p) - SZ(q) + 1;
	Poly res(n);
	for (ll i = n; i--; ){
		res[i] = p.back() / q.back();
		fore(j, 0, SZ(q)) p[j + i] -= res[i] * q[j];
		p.pop_back();
	}
	while (!p.empty() && abs(p.back()) < eps) p.pop_back();
	return {res, p};
}
