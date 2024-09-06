#include "../utilities/template.h"
#include "./utilities.h"

#include "../../content/geometry/Point.h"
#include "../../content/geometry/sideOf.h"

namespace testll {

	#include "../../content/geometry/ConvexHull.h"
	#include "../../content/geometry/MinkowskiSum.h"

	vector<P> slowMinkowskiSum(vector<P>& p, vector<P>& q) {
		vector<P> pq;
		for (P pp : p) for (P qq : q) {
			pq.pb(pp + qq);
		}
		vector<P> ans = convexHull(pq);
		return ans;
	}

	void testCase() {
		ll n = rand() % 100 + 1, m = rand() % 100 + 1;

		vector<P> p, q;
		fore(_, 0, n) {
			ll x = rand() % 1000 - 500, y = rand() % 1000 - 500;
			p.pb(P(x, y));
		}
		fore(_, 0, m) {
			ll x = rand() % 1000 - 500, y = rand() % 1000 - 500;
			q.pb(P(x, y));
		}

		p = convexHull(p);
		q = convexHull(q);

		vector<P> ans = minkowskiSum(p, q);
		assert(isConvexCCW(ans));
		vector<P> slow_ans = slowMinkowskiSum(p, q);
		assert(polygonEq(ans, slow_ans));
	}

} // namespace testll

namespace testdouble {

	const double eps = 1e-9;

	typedef Point<double> P;

	bool eq(P a, P b) {
		return (a - b).dist() < eps;
	}

	/// content/geometry/ConvexHull.h
	/// START diff
	/// END diff
	vector<P> convexHull(vector<P> pts) {
		if (SZ(pts) <= 1) return pts;
		sort(ALL(pts));
		vector<P> h(SZ(pts)+1);
		ll s = 0, t = 0;
		for (ll it = 2; it--; s = --t, reverse(ALL(pts)))
			for (P p : pts) {
				/// START diff
				while (t >= s + 2 && h[t-2].cross(h[t-1], p) <= eps) t--;
				/// END diff
				h[t++] = p;
			}
		/// START diff
		return {h.begin(), h.begin() + t - (t == 2 && eq(h[0], h[1]))};
		/// END diff
	}
	/// END content

	/// content/geometry/MinkowskiSum.h
	/// START diff
	/// END diff
	void reorder(vector<P> &p) {
		/// START diff
		if (sideOf(p[0], p[1], p[2], eps) < 0) reverse(ALL(p));
		/// END diff
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
			/// START diff
			double cross = (p[i + 1] - p[i]).cross(q[j + 1] - q[j]);
			i += cross >= -eps, j += cross <= eps;
			/// END diff
		}
		return r;
	}
	/// END content

	vector<P> slowMinkowskiSum(vector<P>& p, vector<P>& q) {
		vector<P> pq;
		for (P pp : p) for (P qq : q) {
			pq.pb(pp + qq);
		}
		vector<P> ans = convexHull(pq);
		return ans;
	}

	void testCase() {
		ll n = rand() % 100 + 1, m = rand() % 100 + 1;

		vector<P> p, q;
		fore(_, 0, n) {
			double x = (double)(rand() % 1000 - 500) / 10.0, y = (double)(rand() % 1000 - 500) / 10.0;
			p.pb(P(x, y));
		}
		fore(_, 0, m) {
			double x = (double)(rand() % 1000 - 500) / 10.0, y = (double)(rand() % 1000 - 500) / 10.0;
			q.pb(P(x, y));
		}

		p = convexHull(p);
		q = convexHull(q);

		vector<P> ans = minkowskiSum(p, q);
		assert(isConvexCCW(ans, eps));
		vector<P> slow_ans = slowMinkowskiSum(p, q);
		assert(polygonEq(ans, slow_ans, eps));
	}
}  // namespace testdouble

int main() {
	fore(_, 0, 10000) {
		testll::testCase();
	}
	fore(_, 0, 10000) {
		testdouble::testCase();
	}
	cout << "Tests passed!" << endl;
}
