#include "../utilities/template.h"

const double EPS = 1e-8;
#include "../utilities/genPolygon.h"
#include "../../content/geometry/InsidePolygon.h"
namespace old {

#include "../../content/geometry/OnSegment.h"
#include "../../content/geometry/SegmentDistance.h"

template<class It, class P>
bool insidePolygon(It begin, It end, const P& p,
		bool strict = true) {
	ll n = 0; //number of isects with line from p to (inf,p.y)
	for (It i = begin, j = end-1; i != end; j = i++) {
		//if p is on edge of polygon
		if (onSegment(*i, *j, p)) return !strict;
		//or: if (segDist(*i, *j, p) <= epsilon) return !strict;
		//increment n if segment intersects line from p
		n += (max(i->y,j->y) > p.y && min(i->y,j->y) <= p.y &&
				((*j-*i).cross(p-*i) > 0) == (i->y <= p.y));
	}
	return n&1; //inside if odd number of intersections
}
}
typedef Point<double> P;
bool eq(P a, P b) {
	return (a-b).dist()<EPS;
}
const ll NUMPOLY=100;
const ll PTPERPOLY=100;
void test(ll numPts, ll range) {
	fore(i,0,NUMPOLY) {
		vector<P> poly;
		fore(j,0, numPts)
			poly.pb(P(rand()%range, rand()%range));
		poly = genPolygon(poly);
		fore(i,0,PTPERPOLY) {
			P p(rand()%range, rand()%range);
			assert(inPolygon(poly, p, true) == old::insidePolygon(ALL(poly), p, true));
			assert(inPolygon(poly, p, false) == old::insidePolygon(ALL(poly), p, false));
		}
	}

}
int main() {
	test(20,5);
	test(1001,100);
	test(1000,1000);
	cout << "Tests passed!" << endl;
}
