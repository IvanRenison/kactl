#include "../utilities/template.h"
#include "./utilities.h"

#include "../../content/geometry/HalfplaneIntersection.h"
#include "../../content/geometry/ConvexHull.h"

#pragma GCC optimize ("trapv")

// Test against brote force with fractions
namespace slow {

	typedef __int128_t i128;

	struct frac {
		i128 num, den;
		frac(i128 num_ = 0, i128 den_ = 1) : num(num_), den(den_) {
			i128 g = __gcd(num, den);
			num /= g;
			den /= g;
			if (den < 0) {
				num = -num;
				den = -den;
			}
			assert(den > 0);
		}
		bool operator<(frac f) const {
			return num * f.den < f.num * den;
		}
		bool operator==(frac f) const {
			return num == f.num && den == f.den;
		}
		bool operator<=(frac f) const {
			return num * f.den <= f.num * den;
		}
		bool operator>(frac f) const {
			return num * f.den > f.num * den;
		}
		frac operator+(frac o) const {
			return frac(num * o.den + o.num * den, den * o.den);
		}
		frac operator-(frac o) const {
			return frac(num * o.den - o.num * den, den * o.den);
		}
		frac operator*(frac o) const {
			return frac(num * o.num, den * o.den);
		}
		frac operator/(frac o) const {
			return frac(num * o.den, den * o.num);
		}
	};

	typedef Point<frac> Pf;

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
				ll side = sideOf(s, e, p);
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
}

typedef slow::Pf Pf;

P Pf_to_P(Pf p) {
	return P((double)p.x.num / p.x.den, (double)p.y.num / p.y.den);
}

Pf randIntPt(ll lim) {
	return Pf{rand() % (2 * lim + 1) - lim, rand() % (2 * lim + 1) - lim};
}

slow::frac randFrac(ll lim) {
	ll den = rand() % lim + 1;
	ll num = rand() % den;
	return slow::frac(num, den);
}

Pf randDoublePt(ll lim) {
	Pf ans = randIntPt(lim);
	ans.x = ans.x + randFrac(lim / 2);
	ans.y = ans.y + randFrac(lim / 2);
	return ans;
}


const slow::frac INF = 500;
void addInf(vector<pair<Pf,Pf>> &ans, slow::frac INF = INF) {
	slow::frac nINF = slow::frac() - INF;
	vector<Pf> infPts({Pf(INF, INF), Pf(nINF, INF), Pf(nINF, nINF), Pf(INF, nINF)});
	fore(i, 0, 4) {
		ans.push_back({infPts[i], infPts[(i + 1) % 4]});
	}
}

void test(const vector<pair<Pf,Pf>>& t) {
	const double eps = 1e-11;
	vector<Line> t_(SZ(t));
	fore(i, 0, SZ(t)) {
		t_[i] = {Pf_to_P(t[i].first), Pf_to_P(t[i].second)};
	}
	vector<P> ans = halfPlaneIntersection(t_);
	assert(isConvexCCW(ans, eps));
	ans = convexHull(ans); // Remove colinear
	vector<Pf> ansf = slow::slow(t);
	vector<P> ans2(SZ(ansf));
	fore(i, 0, SZ(ansf)) {
		ans2[i] = Pf_to_P(ansf[i]);
	}
	assert(polygonEq(ans, ans2, eps));
}

void testRandomInt() {
	ll n = rand() % 10 + 1;
	vector<pair<Pf,Pf>> t;
	fore(i, 0, n) {
		Pf p = randIntPt(10);
		Pf q = randIntPt(10);
		if (p == q) continue;
		t.push_back({p, q});
	}
	addInf(t);
	test(t);
}

void testRandomDouble() {
	ll n = rand() % 10 + 1;
	vector<pair<Pf,Pf>> t;
	fore(i, 0, n) {
		Pf p = randDoublePt(10);
		Pf q = randDoublePt(10);
		if (p == q) continue;
		t.push_back({p, q});
	}
	addInf(t);
	test(t);
}

int main() {

	vector<vector<pair<Pf,Pf>>> handmade = {
		{{Pf(0, 0), Pf(5, 0)}, {Pf(5, -2), Pf(5, 2)}, {Pf(5, 2), Pf(2, 2)}, {Pf(0, 3), Pf(0, -3)}},
		{{Pf(0, 0), Pf(5, 0)}},
		{{Pf(0, 0), Pf(5, 0)}, {Pf(5, 0), Pf(0, 0)}}, // Line
		{{Pf(0, 0), Pf(5, 0)}, {Pf(5, 0), Pf(0, 0)}, {Pf(0, 0), Pf(0, 5)}, {Pf(0, 5), Pf(0, 0)}}, // Point
		{{Pf(0, 0), Pf(5, 0)}, {Pf(5, 0), Pf(0, 0)}, {Pf(0, 0), Pf(0, 5)}, {Pf(0, 5), Pf(0, 0)}, {Pf(0, 2), Pf(5, 2)}}, // Empty
		{{Pf(0, 0), Pf(1, 0)}, {Pf(0, 0), Pf(2, 0)}, {Pf(0, 0), Pf(3, 0)}}
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
