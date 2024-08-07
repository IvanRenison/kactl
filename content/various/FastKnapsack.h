/**
 * Author: Mårten Wiman
 * License: CC0
 * Source: Pisinger 1999, "Linear Time Algorithms for Knapsack Problems with Bounded Weights"
 * Description: Given N non-negative integer weights w and a non-negative target t,
 * computes the maximum S <= t such that S is the sum of some subset of the weights.
 * Time: O(N \max(w_i))
 * Status: Tested on kattis:eavesdropperevasion, stress-tested
 */
#pragma once

ll knapsack(vi w, ll t) {
	ll a = 0, b = 0, x;
	while (b < SZ(w) && a + w[b] <= t) a += w[b++];
	if (b == SZ(w)) return a;
	ll m = *max_element(ALL(w));
	vi u, v(2*m, -1);
	v[a+m-t] = b;
	fore(i,b,SZ(w)) {
		u = v;
		fore(x,0,m) v[x+w[i]] = max(v[x+w[i]], u[x]);
		for (x = 2*m; --x > m;) fore(j, max(0ll,u[x]), v[x])
			v[x-w[j]] = max(v[x-w[j]], j);
	}
	for (a = t; v[a+m-t] < 0; a--) ;
	return a;
}
