#include "../utilities/template.h"

#include "../../content/geometry/PolygonArea.h"
#include "../../content/geometry/PolygonCut.h"
#include "../../content/geometry/sideOf.h"
#include "../../content/geometry/InsidePolygon.h"
#include "../../content/geometry/SegmentIntersection.h"

typedef Point<double> P;
int main() {
	fore(it,0,500) {
		ll N = rand() % 10 + 3;
		vector<P> ps;
		fore(i,0,N) ps.pb(P{rand() % 10 - 5, rand() % 10 - 5});
		P p(rand() % 10 - 5), q(rand() % 10 - 5);
		fore(i,0,N) fore(j,i+1,N) {
			P a = ps[i], b = ps[(i+1)%N];
			P c = ps[j], d = ps[(j+1)%N];
			P r1, r2;
			auto r = segInter(a, b, c, d);
			if (SZ(r) == 2) goto fail;
			if (SZ(r) == 1) {
				if (i+1 == j || (j+1) % N == i) ;
				else goto fail;
			}
		}
		if (p == q) { fail: continue; }

		ll count = 0;
		const ll ITS = 400000;
		fore(it,0,ITS) {
			double x = rand() / (RAND_MAX + 1.0) * 10 - 5;
			double y = rand() / (RAND_MAX + 1.0) * 10 - 5;
			if (!inPolygon(ps, P{x,y}, true)) continue;
			if (sideOf(p, q, P{x,y}) > 0) continue;
			count++;
		}
		double approxArea = (double)count / ITS * 100;

		ps = polygonCut(ps, p, q);
		double realArea = ps.empty() ? 0.0 : abs(polygonArea2(ps) / 2.0);

		// cout << setprecision(2) << fixed;
		assert(realArea - approxArea < 2e-1);
		// cout << N << ' ' << realArea << '\t' << approxArea << '\t' << realArea - approxArea << endl;

		// cerr << N << endl;
		// for (auto &x : ps) {
			// cout << x.x << ' ' << x.y << endl;
		// }
	}
	cout << "Tests passed!" << endl;
}
