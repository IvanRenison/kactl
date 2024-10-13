/**
 * Author: Mattias de Zalenski
 * Date: Unknown
 * Source: Geometry in C
 * Description: Computes the Delaunay triangulation of a set of points.
 *  Each circumcircle contains none of the input points.
 *  If any three points are collinear or any four are on the same circle, behavior is undefined.
 * Time: O(n^2)
 * Status: stress-tested
 */
#pragma once

#include "Point.h"
#include "3dHull.h"

template<class P> void delaunay(vector<P>& ps, auto f) {
	if (SZ(ps)==3){ll d=ps[0].cross(ps[1],ps[2])<0;f(0,1+d,2-d);}
	vector<P3> p3;
	for (P p : ps) p3.pb(P3{p.x, p.y, p.dist2()});
	if (SZ(ps) > 3) for (auto [_, a, b, c] : hull3d(p3))
		if ((p3[b]-p3[a]).cross(p3[c]-p3[a]).dot(P3(0,0,1)) < 0)
			f(a, c, b);
}
