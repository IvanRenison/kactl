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
#define sp(a) a.fst, a.snd
#define ang(a) (a.snd - a.fst).angle()

ll angDiff(Line a, Line b) { return sgn(ang(a) - ang(b)); }
bool cmp(Line a, Line b) {
	ll s = angDiff(a, b);
	return (s ? s : sideOf(sp(a), b.fst)) < 0;
}
vector<P> halfPlaneIntersection(vector<Line> vs) {
	const double eps = sqrt(2) * 1e-8;
	sort(ALL(vs), cmp);
	vector<Line> deq(SZ(vs) + 5);
	vector<P> ans(SZ(vs) + 5);
	deq[0] = vs[0];
	ll ah = 0, at = 0, n = SZ(vs);
	fore(i,1,n+1) {
		if (i == n) vs.push_back(deq[ah]);
		if (angDiff(vs[i], vs[i - 1]) == 0) continue;
		while (ah<at && sideOf(sp(vs[i]), ans[at-1], eps) < 0)
			at--;
		while (i!=n && ah<at && sideOf(sp(vs[i]),ans[ah],eps)<0)
			ah++;
		auto res = lineInter(sp(vs[i]), sp(deq[at]));
		if (res.first != 1) continue;
		ans[at++] = res.second, deq[at] = vs[i];
	}
	if (at - ah <= 2) return {};
	return {ans.begin() + ah, ans.begin() + at};
}