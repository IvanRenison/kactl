/**
 * Author: Stjepan Glavina, chilli
 * Date: 2019-05-05
 * License: Unlicense
 * Source: https://github.com/stjepang/snippets/blob/master/convex_hull.cpp
 * Description:
 * \descriptionimage{content/geometry/ConvexHull}
 * Returns a vector of the points of the convex hull in counter-clockwise order.
 * Points on the edge of the hull between two other points are not considered part of the hull.
 * Time: O(n \log n)
 * Status: stress-tested, tested with kattis:convexhull
*/
#pragma once

#include "Point.h"

template<class P>
vector<P> convexHull(vector<P> pts) {
	if (SZ(pts) <= 1) return pts;
	sort(ALL(pts));
	vector<P> h(SZ(pts)+1);
	ll s = 0, t = 0;
	for (ll it = 2; it--; s = --t, reverse(ALL(pts)))
		for (P p : pts) {
			while (t >= s + 2 && h[t-2].cross(h[t-1], p) <= 0) t--;
			h[t++] = p;
		}
	return {h.begin(), h.begin() + t - (t == 2 && h[0] == h[1])};
}
