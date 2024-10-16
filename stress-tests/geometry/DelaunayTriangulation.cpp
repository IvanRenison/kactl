#include "../utilities/template.h"

#include "../../content/geometry/DelaunayTriangulation.h"
#define ll double
#include "../../content/geometry/ConvexHull.h"
#undef ll
#include "../../content/geometry/PolygonArea.h"
#include "../../content/geometry/circumcircle.h"

typedef Point<double> P;
int main() {
	feenableexcept(29);
	fore(it,0,100000) {{
		vector<P> ps;
		ll N = rand() % 20 + 1;
		fore(i,0,N) {
			ps.pb(P{rand() % 100 - 50, rand() % 100 - 50});
		}

		auto coc = [&](ll i, ll j, ll k, ll l) {
			double a = (ps[i] - ps[j]).dist();
			double b = (ps[j] - ps[k]).dist();
			double c = (ps[k] - ps[l]).dist();
			double d = (ps[l] - ps[i]).dist();
			double e = (ps[i] - ps[k]).dist();
			double f = (ps[j] - ps[l]).dist();
			double q = a*c + b*d - e*f;
			return abs(q) < 1e-4;
		};

		fore(i,0,N) fore(j,0,i) fore(k,0,j) {
			if (ps[i].cross(ps[j], ps[k]) == 0) {  goto fail; }
		}
		fore(i,0,N) fore(j,0,i) fore(k,0,j) fore(l,0,k) {
			if (coc(i,j,k,l) || coc(i,j,l,k) || coc(i,l,j,k) || coc(i,l,k,j)) { goto fail; }
		}

		auto fail = [&]() {
			cout << "Points:" << endl;
			for (auto &p : ps) {
				cout << p.x << ' ' << p.y << endl;
			}

			cout << "Triangles:" << endl;
			delaunay(ps, [&](ll i, ll j, ll k) {
				cout << i << ' ' << j << ' ' << k << endl;
			});

			abort();
		};

		double sumar = 0;
		vi used(N);
		delaunay(ps, [&](ll i, ll j, ll k) {
			used[i] = used[j] = used[k] = 1;
			double ar = ps[i].cross(ps[j], ps[k]);
			if (ar < -1e-4) fail();
			sumar += ar;
			P c = ccCenter(ps[i], ps[j], ps[k]);
			double ra = ccRadius(ps[i], ps[j], ps[k]);
			fore(l,0,N) {
				if ((ps[l] - c).dist() < ra - 1e-5) fail();
			}
		});
		if (N >= 3) fore(i,0,N) if (!used[i]) fail();

		vector<P> hull = convexHull(ps);
		double ar2 = polygonArea2(hull);
		if (abs(sumar - ar2) > 1e-4) fail();

		continue; }
fail:;
	}
	cout << "Tests passed!" << endl;
}
