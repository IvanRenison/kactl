/**
 * Author: Per Austrin
 * Date: 2004-02-08
 * License: CC0
 * Description: Finds the real roots to a polynomial.
 * Usage: polyRoots({{2,-3,1}},-1e9,1e9) // solve x^2-3x+2 = 0
 * Time: O(n^2 \log(1/\epsilon))
 * Status: untested
 */
#pragma once

#include "Polynomial.h"

vector<double> polyRoots(Poly& p, double xmin, double xmax) {
	if (SZ(p) == 2) return {-p[0]/p[1]};
	vector<double> ret;
	Poly der = derivate(p), dr = polyRoots(der, xmin, xmax);
	dr.pb(xmin-1), dr.pb(xmax+1);
	sort(ALL(dr));
	fore(i,0,SZ(dr)-1) {
		double l = dr[i], h = dr[i+1];
		bool sign = eval(p, l) > 0;
		if (sign ^ (eval(p, h) > 0)) {
			fore(it, 0, 60) { // while (h - l > 1e-8)
				double m = (l + h) / 2, f = eval(p, m);
				if ((f <= 0) ^ sign) l = m;
				else h = m;
			}
			ret.pb((l + h) / 2);
		}
	}
	return ret;
}
