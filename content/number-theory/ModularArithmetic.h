/**
 * Author: Lukas Polacek
 * Date: 2009-09-28
 * License: CC0
 * Source: folklore
 * Description: Operators for modular arithmetic. Update mod.
 * Use commented code in invert if mod is not prime.
 */
#pragma once

#include "euclid.h"

const ll mod = 17; // change to something else
struct Mod {
	ll x;
	Mod(ll xx) : x(xx) {}
	Mod operator+(Mod b) { return Mod((x + b.x) % mod); }
	Mod operator-(Mod b) { return Mod((x - b.x + mod) % mod); }
	Mod operator*(Mod b) { return Mod((x * b.x) % mod); }
	Mod operator/(Mod b) { return *this * invert(b); }
	Mod invert(Mod a) {
		return a ^ (mod - 2);
		// ll x, y, g = euclid(a.x, mod, x, y);
		// assert(g == 1); return Mod((x + mod) % mod);
	}
	Mod operator^(ll e) {
		Mod ans(1);
		for (Mod b = *this; e; b = b * b, e >>= 1)
			if (e & 1) ans = ans * b;
		return ans;
	}
};
