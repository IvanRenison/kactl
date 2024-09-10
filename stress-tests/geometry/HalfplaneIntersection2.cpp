#include "../utilities/template.h"
#include "../utilities/randGeo.h"
#include "./utilities.h"

#include "../../content/geometry/HalfplaneIntersection.h"
#include "../../content/geometry/ConvexHull.h"

const double eps = 1e-11;

// Test against brote force with float128
typedef Point<__float128> Pf;
vector<Pf> slow(const vector<pair<Pf,Pf>>& t) {

	ll n = SZ(t);
	vector<Pf> points;
	fore(i, 0, n) fore(j, 0, i) {
		auto [si, ei] = t[i];
		auto [sj, ej] = t[j];
		auto [x, p] = lineInter(si, ei, sj, ej);
		if (x == 1) {
			points.push_back(p);
		}
	}

	vector<Pf> ans;
	for (Pf p : points) {
		bool valid = true;
		for (auto [s, e] : t) {
			ll side = sideOf(s, e, p, eps);
			if (side == -1) {
				valid = false;
				break;
			}
		}
		if (valid) {
			ans.push_back(p);
		}
	}

	ans = convexHull(ans);
	return ans;
}

const double INF = 1000;
void addInf(vector<Line> &ans, double INF = INF) {
	vector<P> infPts({P(INF, INF), P(-INF, INF), P(-INF, -INF), P(INF, -INF)});
	fore(i, 0, 4) {
		ans.push_back({infPts[i], infPts[(i + 1) % 4]});
	}
}

void test(const vector<Line>& t) {
	vector<P> ans = halfPlaneIntersection(t);
	assert(isConvexCCW(ans, eps));
	ans = convexHull(ans); // Remove colinear
	vector<pair<Pf,Pf>> t2(SZ(t));
	fore(i, 0, SZ(t)) {
		auto [p, q, _] = t[i];
		auto [px, py] = p;
		auto [qx, qy] = q;
		t2[i] = {Pf(px, py), Pf(qx, qy)};
	}
	vector<Pf> ans2_ = slow(t2);
	vector<P> ans2(SZ(ans2_));
	fore(i, 0, SZ(ans2_)) {
		auto [x, y] = ans2_[i];
		ans2[i] = P((double)x, (double)y);
	}
	assert(polygonEq(ans, ans2, eps));
}

void testRandomInt() {
	ll n = rand() % 100 + 1;
	vector<Line> t;
	fore(i, 0, n) {
		P p = randIntPt(100);
		P q = randIntPt(100);
		if (p == q) continue;
		t.push_back({p, q});
	}
	addInf(t);
	test(t);
}

void testRandomDouble() {
	ll n = rand() % 100 + 1;
	vector<Line> t;
	fore(i, 0, n) {
		P p = randDoublePt(100);
		P q = randDoublePt(100);
		if ((p - q).dist2() < eps) continue;
		t.push_back({p, q});
	}
	addInf(t);
	test(t);
}

int main() {

	vector<vector<Line>> handmade = {
		{{P(0, 0), P(5, 0)}, {P(5, -2), P(5, 2)}, {P(5, 2), P(2, 2)}, {P(0, 3), P(0, -3)}},
		{{P(0, 0), P(5, 0)}},
		{{P(0, 0), P(5, 0)}, {P(5, 0), P(0, 0)}}, // Line
		{{P(0, 0), P(5, 0)}, {P(5, 0), P(0, 0)}, {P(0, 0), P(0, 5)}, {P(0, 5), P(0, 0)}}, // Point
		{{P(0, 0), P(5, 0)}, {P(5, 0), P(0, 0)}, {P(0, 0), P(0, 5)}, {P(0, 5), P(0, 0)}, {P(0, 2), P(5, 2)}}, // Empty
		{{P(0, 0), P(5, 0)}, {P(5, 0), P(5, 5)}, {P(5, 5), P(0, 5)}, {P(0, 5), P(0, 0)}, {P(1, 5), P(1, 0)}}, // Parallel lines
		{{P(0, 0), P(5, 0)}, {P(5, 0), P(5, 5)}, {P(5, 5), P(0, 5)}, {P(1, 5), P(1, 0)}, {P(0, 5), P(0, 0)}} // Parallel lines
	};

	for (auto& t : handmade) {
		addInf(t);
		test(t);
	}

	fore(_, 0, 1000) {
		testRandomInt();
		testRandomDouble();
	}

	cout << "Tests passed!" << endl;
}
