/**
 * Author: Ulf Lundstrom
 * Date: 2009-04-08
 * License: CC0
 * Source:
 * Description: Returns the center of mass for a polygon.
 * Time: O(n)
 * Status: Tested
 */
#pragma once

#include "Point.h"

typedef Point<double> P;
P polygonCenter(const vector<P>& v) {
	P res(0, 0); double A = 0;
	for (ll i = 0, j = SZ(v) - 1; i < SZ(v); j = i++) {
		res = res + (v[i] + v[j]) * v[j].cross(v[i]);
		A += v[j].cross(v[i]);
	}
	return res / A / 3;
}
