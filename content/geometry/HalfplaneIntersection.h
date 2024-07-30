/**
 * Author: chilli and Iván Renison
 * Date: 2019-05-05
 * License: CC0
 * Source: https://github.com/zimpha/algorithmic-library/blob/master/computational-geometry/polygon.cc
 * Description: Computes the intersection of a set of half-planes. Input is given as a set of planes, facing left.
 * Output is the convex polygon representing the intersection. The points may have duplicates and be collinear.
 * Will not fail catastrophically if `eps > sqrt(2)(line intersection error)`. Likely to work for more ranges if
 * 3 half planes are never guaranteed to intersect at the same point.
 * Time: O(n \log n)
 * Status: stress-tested
 */
#pragma once

#include "Point.h"
#include "sideOf.h"
#include "lineIntersection.h"

typedef Point<double> P;
typedef pair<P, P> Line;
#define L(a) a.fst, a.snd

ll angDiff(Line a, Line b) {
	return sgn((a.snd-a.fst).angle() - (b.snd-b.fst).angle());
}
vector<P> halfPlaneIntersection(vector<Line> v) {
	const double eps = sqrt(2) * 1e-8;
	sort(ALL(v), [&](Line a, Line b) {
		ll s = angDiff(a, b);
		return (s ? s : sideOf(L(a), b.fst)) < 0;
	});
	ll ah = 0, at = 0, n = SZ(v);
	vector<Line> deq(n + 1);
	vector<P> ans(n);
	deq[0] = v[0];
	fore(i, 1, n + 1) {
		if (i == n) v.pb(deq[ah]);
		if (angDiff(v[i], v[i - 1])) {
			while (ah < at && sideOf(L(v[i]), ans[at-1], eps) < 0)
				at--;
			while (i < n && ah < at && sideOf(L(v[i]),ans[ah],eps)<0)
				ah++;
			auto [r, p] = lineInter(L(v[i]), L(deq[at]));
			if (r == 1) ans[at++] = p, deq[at] = v[i];
		}
	}
	if (at - ah < 3) return {};
	return {ans.begin() + ah, ans.begin() + at};
}