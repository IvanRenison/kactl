#include "../utilities/template.h"

#include "../../content/geometry/CirclePolygonIntersection.h"
#include "../utilities/genPolygon.h"

namespace orig{
typedef Point<long double> P;
long double areaCT(P pa, P pb, long double r) {
	if (pa.dist() < pb.dist()) swap(pa, pb);
	if (sgn(pb.dist()) == 0) return 0;
	long double a = pb.dist(), b = pa.dist(), c = (pb - pa).dist();
	long double sinB = fabs(pb.cross(pb - pa) / a / c), cosB = pb.dot(pb - pa) / a / c,
		sinC = fabs(pa.cross(pb) / a / b), cosC = pa.dot(pb) / a / b;
	long double B = atan2(sinB, cosB), C = atan2(sinC, cosC);
	if (a > r) {
		long double S = C / 2 * r * r, h = a * b * sinC / c;
		if (h < r && B < M_PI / 2)
			S -= (acos(h / r) * r * r - h * sqrt(r * r - h * h));
		return S;
	} else if (b > r) {
		long double theta = M_PI - B - asin(sinB / r * a);
		return a * r * sin(theta) / 2 + (C - theta) / 2 * r * r;
	} else return sinC * a * b / 2;
}
long double circlePoly(P c, long double r, vector<P> poly) {
	long double area = 0;
	fore(i,0,SZ(poly)) {
		auto a = poly[i] - c, b = poly[(i+1)%SZ(poly)] - c;
		area += areaCT(a, b, r) * sgn(a.cross(b));
	}
	return area;
}
}

signed main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	const ll lim=5;
	fore(i, 0, 100000) {

		vector<Point<ll>> pts;
		fore(j, 0, 10) {
			ll x = rand()%lim, y = rand()%lim;
			pts.pb(Point<ll>(x, y));
		}

		auto polyInt = genPolygon(pts);

		ll cx = rand()%lim, cy = rand()%lim;
		auto c = P(cx, cy);
		auto c2 = orig::P(cx, cy);
		double r= rand()%(2*lim);

		vector<P> poly;
		vector<orig::P> poly2;
		for (auto [x, y] : polyInt) {
			poly.pb(P(x, y));
			poly2.pb(orig::P(x, y));
		}
		auto res1 = circlePoly(c, r, poly);
		auto res2 = orig::circlePoly(c2, r, poly2);

		if (abs(res1 - res2) > 1e-8) {
			cout<<abs(res1-res2)<<' '<<res1<<' '<<res2<<endl;
			assert(false);
		}
	}
	cout << "Tests passed!" << endl;
}
