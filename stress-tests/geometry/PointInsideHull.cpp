#include "../utilities/template.h"

#include "../../content/geometry/ConvexHull.h"
namespace ignore {
	#include "../../content/geometry/SegmentDistance.h"
}
#include "../../content/geometry/PointInsideHull.h"
#include "../../content/geometry/InsidePolygon.h"

int main() {
	fore(it,0,100000) {
		ll N = rand() % 15;
		vector<P> ps;
		fore(i,0,N) ps.pb(P{rand() % 20 - 10, rand() % 20 - 10});
		vector<P> ps2 = convexHull(ps);
		if (ps2.empty()) continue;
		fore(it2,0,20) {
			ll x = rand() % 22 - 11;
			ll y = rand() % 22 - 11;
			P p{x,y};
			assert(inPolygon(ps2, p, true) == (inHull(ps2, p, true)));
			assert(inPolygon(ps2, p, false) == (inHull(ps2, p, false)));
		}
	}
	cout << "Tests passed!" << endl;
}
