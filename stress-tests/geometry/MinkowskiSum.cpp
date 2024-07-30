#include "../utilities/template.h"

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

// Test if the two polygons are the same set of points
bool areEq(vector<P>& p, vector<P>& q) {
	return set<P>(ALL(p)) == set<P>(ALL(q));
}

void testCasell() {
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
	vector<P> slow_ans = slowMinkowskiSum(p, q);
	assert(areEq(ans, slow_ans));
}

int main() {
	fore(_, 0, 100000) {
		testCasell();
	}
	cout << "Tests passed!" << endl;
}


