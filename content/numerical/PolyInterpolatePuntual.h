/**
 * Author: Iván Renison
 * Date: 2024-08-30
 * License: CC0
 * Source: me
 * Description: Given for $i \in \{0, 1, ..., n-1\} $ points $(i, y[i])$ computes the value
 * of the n-1-degree polynomial that passes through them at point $x$.
 * Time: O(n \log mod)
 * Status: problem tested
 */
#pragma once

#include "../number-theory/ModularArithmetic.h"

Mod interpolate(vector<Mod>& y, Mod x) {
	ll n = SZ(y);
	static vector<Mod> fi {1}; // Inverses of factorials
	while (SZ(fi) < n) fi.pb(fi.back() / SZ(fi));
	if (x.x < n) return y[x.x];
	Mod p = 1;
	fore(i, 0, n) p = p * (x - i);
	Mod ans = 0;
	fore(i, 0, n) {
		Mod t = y[i] * p / (x-i) * fi[i] * fi[n-1-i];
		if ((n-i) % 2 == 0) t = t * (mod - 1);
		ans = ans + t;
	}
	return ans;
}
