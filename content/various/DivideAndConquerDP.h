/**
 * Author: Simon Lindholm
 * License: CC0
 * Source: Codeforces
 * Description: Given $a[i] = \min_{lo(i) \le k < hi(i)}(f(i, k))$ where the (minimal)
 * optimal $k$ increases with $i$, computes $a[i]$ for $i = L..R-1$.
 * Time: O((N + (hi-lo)) \log N)
 * Status: tested on http://codeforces.com/contest/321/problem/E
 */
#pragma once

struct DP { // Modify at will:
	ll lo(ll ind) { return 0; }
	ll hi(ll ind) { return ind; }
	ll f(ll ind, ll k) { return dp[ind][k]; }
	void store(ll ind, ll k, ll v) { res[ind] = {k, v}; }

	void rec(ll L, ll R, ll LO, ll HI) {
		if (L >= R) return;
		ll mid = (L + R) >> 1;
		ii best(LLONG_MAX, LO);
		for(ll k = max(LO, lo(mid)); k < min(HI, hi(mid)); k++)
			best = min(best, make_pair(f(mid, k), k));
		store(mid, best.snd, best.fst);
		rec(L, mid, LO, best.snd+1);
		rec(mid+1, R, best.snd, HI);
	}
	void solve(ll L, ll R) { rec(L, R, LLONG_MIN, LLONG_MAX); }
};
