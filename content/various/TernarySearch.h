/**
 * Author: Simon Lindholm
 * Date: 2015-05-12
 * License: CC0
 * Source: own work
 * Description:
 * Find the smallest i in $[a,b]$ that maximizes $f(i)$, assuming that $f(a) < \dots < f(i) \ge \dots \ge f(b)$.
 * To reverse which of the sides allows non-strict inequalities, change the < marked with (A) to <=, and reverse the loop at (B).
 * To minimize $f$, change it to >, also at (B).
 * Usage:
	ll ind = ternSearch(0,n-1,[\&](ll i){return a[i];});
 * Time: O(\log(b-a))
 * Status: tested
 */
#pragma once

ll ternSearch(ll a, ll b, auto f) {
	assert(a <= b);
	while (b - a >= 5) {
		ll mid = (a + b) / 2;
		if (f(mid) < f(mid+1)) a = mid; // (A)
		else b = mid+1;
	}
	fore(i,a+1,b+1) if (f(a) < f(i)) a = i; // (B)
	return a;
}
