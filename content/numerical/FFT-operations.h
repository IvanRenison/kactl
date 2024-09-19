/**
 * Author: Iván Renison
 * Date: 2024-09-19
 * License: CC0
 * Source: notebook el vasito
 * Description:
 * Time:
 * Status: Untested, probably not okey
 */
#pragma once

#include "FastFourierTransform.h"
#include "Polynomial.h"

Poly takeMod(Poly p, ll n) { // O(n)
	p.resize(min(SZ(p), n));   // p % (x^n)
	while(!p.empty() && abs(p.back()) <= eps) p.pop_back();
	return p;
}

Poly invert(const Poly& p, ll d) { // O(n log(n))
	Poly res = {1/p[0]};             // first d terms of 1/p
	ll sz = 1;
	while (sz < d){
		sz *= 2;
		Poly pre(p.begin(), p.begin() + min(SZ(p), sz));
		Poly cur = conv(res, pre);
		fore(i, 0, SZ(cur)) cur[i] = -cur[i];
		cur[0] = cur[0] + 2;
		res = conv(res, cur);
		res = takeMod(res, sz);
	}
	res.resize(d);
	return res;
}

Poly log(const Poly& p, ll d){ // O(n log(n))
	Poly cur = takeMod(p, d);    // first d terms of log(p)
	Poly a = invert(cur, d), b = derivate(cur);
	auto res = conv(a,b);
	res = takeMod(res, d-1);
	res = integrate(res);
	return res;
}

Poly exp(const Poly& p, ll d) { // O(n log(n)^2)
	Poly res = {1};               // first d terms of exp(p)
	ll sz = 1;
	while (sz < d) {
		sz *= 2;
		Poly lg = log(res, sz), cur(sz);
		fore(i, 0, sz)
			cur[i] = (i<SZ(p) ? p[i] : 0) - (i<SZ(lg) ? lg[i] : 0);
		cur[0] = cur[0] + 1;
		res = conv(res, cur);
		res = takeMod(res, sz);
	}
	res.resize(d);
	return res;
}

pair<Poly,Poly> divide(const Poly& a, const Poly& b){
	ll m = SZ(a), n = SZ(b);   // O(n log(n)), returns {res, rem}
	if (m < n) return {{}, a}; // if min(m-n,n) < 750 it may be
	Poly ap = a, bp = b;       // faster to use cuadratic version
	reverse(ALL(ap));
	reverse(ALL(bp));
	bp = invert(bp, m - n + 1);
	Poly q = conv(ap, bp);
	q.resize(SZ(q) + m - n - SZ(q) + 1, 0);
	reverse(ALL(q));
	Poly bq = conv(b, q);
	fore(i,0,SZ(bq)) bq[i] = -bq[i];
	Poly r = add(a, bq);
	return {q, r};
}

void filltree(vector<double>& x, vector<Poly>& tree) {
	ll k = SZ(x);
	tree.resize(2*k);
	fore(i, k, 2*k) tree[i] = {-x[i - k], 1};
	for(ll i = k; i--;) tree[i] = conv(tree[2*i], tree[2*i+1]);
}

vector<double> evaluate(Poly& a, vector<double>& x) {
	vector<Poly> tree;          // O(n log(n)^2)
	filltree(x, tree);          // Evaluate a in all points of x
	ll k = SZ(x);
	vector<Poly> ans(2*k);
	ans[1] = divide(a, tree[1]).snd;
	fore(i,2,2*k) ans[i] = divide(ans[i>>1], tree[i]).snd;
	vector<double> r;
	fore(i,0,k) r.pb(ans[i+k][0]);
	return r;
}

Poly interpolate(vector<double>& x, vector<double>& y) {
	vector<Poly> tree;                           // O(n log(n)^2)
	filltree(x, tree);
	Poly p = derivate(tree[1]);
	ll k = SZ(y);
	vector<double> d = evaluate(p, x);
	vector<Poly> intree(2*k);
	fore(i, k, 2*k) intree[i] = {y[i-k] / d[i-k]};
	for(ll i = k-1; i; i--) {
		Poly p1 = conv(tree[2*i], intree[2*i+1]);
		Poly p2 = conv(tree[2*i+1], intree[2*i]);
		intree[i] = add(p1,p2);
	}
	return intree[1];
}

