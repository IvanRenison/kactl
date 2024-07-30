/**
 * Author: Iván Renison
 * Date: 2024-07-30
 * License: CC0
 * Source: notebook el vasito
 * Description: Compute Minkowski sum of two strictly convex non empty polygons (i.e. two hulls)
 * Returns answer in counter clock wise order.
 * The Minkowski sum of two polygons P and Q viewed as set of $\mathbb{R}^2$ is defined as $\{p + q : p \in P, q \in Q\}$
 * Time: O(n \log n + m \log m)
 * Status:
*/

#include "Point.h"
#include "ConvexHull.h"
#include "sideOf.h"

typedef Point<ll> P;

void reorder(vector<P> &p){
	if (sideOf(p[0], p[1], p[2]) < 1) reverse(ALL(p));
	rotate(p.begin(), min_element(ALL(p)), p.end());
}
vector<P> minkowskiSum(vector<P> p, vector<P> q) {
	if (min(SZ(p), SZ(q)) < 3) {
	  vector<P> v;
		for (P pp : p) for (P qq : q) v.pb(pp + qq);
		return convexHull(v);
	}
	reorder(p), reorder(q);
	fore(i, 0, 2) p.pb(p[i]), q.pb(q[i]);
	vector<P> r;
	ll i = 0, j = 0;
	while (i + 2 < SZ(p) || j + 2 < SZ(q)) {
		r.pb(p[i] + q[j]);
		ll cross = (p[i + 1] - p[i]).cross(q[j + 1] - q[j]);
		i += cross >= 0, j += cross <= 0;
	}
	return r;
}
