#include "../utilities/template.h"

#include "../../content/geometry/ConvexHull.h"
#include "../../content/geometry/HullDiameter.h"

int main() {
	srand(2);
	fore(it,0,1000000) {
		ll N = (rand() % 10) + 1;
		vector<Point<ll>> ps;
		fore(i,0,N) {
			ps.pb(P{rand() % 11 - 5, rand() % 11 - 5});
		}
		ll r1 = 0;
		fore(i,0,N) fore(j,0,i) {
			r1 = max(r1, (ps[i] - ps[j]).dist2());
		}
		auto pa = hullDiameter(convexHull(ps));
		ll r2 = ps.empty() ? 0LL : (pa[0] - pa[1]).dist2();
		assert(r1 == r2);
	}
	cout << "Tests passed!" << endl;
}
