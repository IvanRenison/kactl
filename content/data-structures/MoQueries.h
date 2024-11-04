/**
 * Author: Pietro Palombini
 * Date: 2024-11-03
 * License: CC0
 * Source: https://codeforces.com/blog/entry/61203/
 * Description: Answer range queries offline using Hilbert
 * order or SQRT decomposition. For Hilbert, change lines with
 * \texttt{SQ} for commented lines. Define \texttt{add},
 * \texttt{rem}, \texttt{calc}, add necessary fields to
 * \texttt{MoQueries}, instantiate, and call \texttt{solve}.
 * Queries are given as pairs $[l, r)$. \texttt{end} is true if
 * the element should be added/removed from the end of the
 * range. \texttt{qid} is the query index. Starts at [0, 0).
 * Time: Fast O(T N \sqrt Q + CQ) where T is the cost of
 * \texttt{add} and \texttt{rem} and C is the cost of
 * \texttt{calc}.
 */

#pragma once

constexpr ll B = 447; // ~N/sqrt(Q)
struct MoQueries {
	typedef ll T;
	void add(ll pos, bool end, ll qid) {}
	void rem(ll pos, bool end, ll qid) {}
	T calc() { return 0; }
	ii k(ii &x) { return {x.fst/B, x.snd ^ -(x.fst/B&1)};} // SQ
	vector<T> solve(ll n, vector<ii> &qs) {
		ll l=0,r=0,q=SZ(qs); //,rx,ry,k,s;
		vi p(q); //,o(q);
		iota(ALL(p), 0);
	//fore(i, 0, q) {
	//	auto [x, y] = qs[i];
	//	for (k = s = bit_ceil((unsigned)n); s >>= 1;) {
	//		rx = (x&s)>0, ry = (y&s)>0, o[i] += s*s*((rx*3)^ry);
	//		if (!ry) {
	//			if (rx) x = k - 1 - x, y = k - 1 - y;
	//			swap(x, y);
	//		}
	//	}
	//}
	//sort(ALL(p), [&](ll i, ll j) { return o[i] < o[j]; });
		sort(ALL(p),[&](ll i,ll j){return k(qs[i])<k(qs[j]);});//SQ
		vector<T> res(q);
		for (ll i : p) {
			auto [ql, qr] = qs[i];
			while (l > ql) add(--l, 0, i);
			while (r < qr) add(r++, 1, i);
			while (l < ql) rem(l++, 0, i);
			while (r > qr) rem(--r, 1, i);
			res[i] = calc();
		}
		return res;
	}
};
