/**
 * Author: Pietro Palombini
 * Date: 2024-10-11
 * Source: notebook el vasito
 * Usage: auto [x, y, dx, dy] = diophantine(a, b, r);
 * Description: All integer solutions (x, y) to $ax + by = r$
 * are of * the form $(x + k \cdot dx, y + k \cdot dy)$ for
 * integer $k$.
 * Time: O(\log(\min(a,b)))
 * Status: stress-tested
 */
#pragma once

#include "./euclid.h"

array<ll, 4> diophantine(ll a, ll b, ll r) {
	ll x, y, g = euclid(a, b, x, y);
	a /= g, b /= g, r /= g, x *= r, y *= r;
	assert(a * x + b * y == r); // otherwise no solution
	return {x, y, -b, a};
}
