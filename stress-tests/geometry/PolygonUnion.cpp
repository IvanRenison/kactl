#include "../utilities/template.h"

#include "../../content/geometry/Point.h"
#include "../../content/geometry/sideOf.h"
#include "../../content/geometry/PolygonArea.h"
#include "../../content/geometry/PolygonUnion.h"
#include "../utilities/genPolygon.h"
#include "../utilities/random.h"

namespace blackhorse {

using db = double;
const db eps = 1e-8;

struct pt {
	db x, y;
	pt(db x = 0, db y = 0) : x(x), y(y) {}
};

inline ll sgn(db x) { return (x > eps) - (x < -eps); }

pt operator-(pt p1, pt p2) { return pt(p1.x - p2.x, p1.y - p2.y); }

db vect(pt p1, pt p2) { return p1.x * p2.y - p1.y * p2.x; }

db scal(pt p1, pt p2) { return p1.x * p2.x + p1.y * p2.y; }

db polygon_union(vector<pt> poly[], ll n) {
	auto ratio = [](pt A, pt B, pt O) {
		return !sgn(A.x - B.x) ? (O.y - A.y) / (B.y - A.y) : (O.x - A.x) / (B.x - A.x);
	};
	db ret = 0;
	for (ll i = 0; i < n; ++i) {
		for (size_t v = 0; v < poly[i].size(); ++v) {
			pt A = poly[i][v], B = poly[i][(v + 1) % poly[i].size()];
			vector<pair<db, ll>> segs;
			segs.pb({0, 0}), segs.pb({1, 0});
			for (ll j = 0; j < n; ++j)
				if (i != j) {
					for (size_t u = 0; u < poly[j].size(); ++u) {
						pt C = poly[j][u], D = poly[j][(u + 1) % poly[j].size()];
						ll sc = sgn(vect(B - A, C - A)), sd = sgn(vect(B - A, D - A));
						if (!sc && !sd) {
							if (sgn(scal(B - A, D - C)) > 0 && i > j) {
								segs.pb({ratio(A, B, C), 1}), segs.pb({ratio(A, B, D), -1});
							}
						} else {
							db sa = vect(D - C, A - C), sb = vect(D - C, B - C);
							if (sc >= 0 && sd < 0)
								segs.pb({sa / (sa - sb), 1});
							else if (sc < 0 && sd >= 0)
								segs.pb({sa / (sa - sb), -1});
						}
					}
				}
			sort(segs.begin(), segs.end());
			db pre = min(max(segs[0].fst, 0.0), 1.0), now, sum = 0;
			ll cnt = segs[0].snd;
			for (size_t j = 1; j < segs.size(); ++j) {
				now = min(max(segs[j].fst, 0.0), 1.0);
				if (!cnt)
					sum += now - pre;
				cnt += segs[j].snd;
				pre = now;
			}
			ret += vect(A, B) * sum;
		}
	}
	return ret / 2;
}
} // namespace blackhorse

namespace approximate {
#include "../../content/geometry/InsidePolygon.h"
double polygonUnion(vector<vector<P>> &polygons, ll lim) {
	ll cnt = 0;
	ll total = 0;
	for (double y = -lim + 1e-5; y < lim; y += lim / 500.0) {
		for (double x = -lim + 1.1e-5; x < lim; x += lim / 500.0) {
			total++;
			for (auto &i : polygons) {
				if (inPolygon(i, P(x, y))) {
					cnt++;
					break;
				}
			}
		}
	}
	return lim * lim * 4 * cnt / double(total);
}
} // namespace approximate

namespace lovelive {
#define re real
#define im imag
#define fir fst
#define sec snd
typedef double db;
const db pi = acos(db(-1));
inline ll sgn(db x) { return (x > 1e-8) - (x < -1e-8); }

typedef complex<db> cpoi;
db polygon_union(vector<cpoi> py[], ll n) {
	auto ratio = [](cpoi &a, cpoi &b, cpoi &c) {
		cpoi x = b - a, y = c - a;
		if (sgn(re(x)) == 0)
			return im(y) / im(x);
		return re(y) / re(x);
	};
	db ret = 0;
	for (ll i = 0; i < n; ++i)
		for (size_t v = 0; v < py[i].size(); ++v) {
			cpoi a = py[i][v], b = py[i][(v + 1) % py[i].size()];
			vector<pair<db, ll>> segs = {{0, 0}, {1, 0}};
			for (ll j = 0; j < n; ++j)
				if (i != j)
					for (size_t u = 0; u < py[j].size(); ++u) {
						cpoi c = py[j][u], d = py[j][(u + 1) % py[j].size()];
						ll sc = sgn(im(conj(b - a) * (c - a)));
						ll sd = sgn(im(conj(b - a) * (d - a)));
						if (!sc && !sd) {
							if (sgn(re(conj(b - a) * (d - c))) > 0 && i > j) {
								segs.pb({ratio(a, b, c), +1});
								segs.pb({ratio(a, b, d), -1});
							}
						} else {
							db sa = im(conj(d - c) * (a - c));
							db sb = im(conj(d - c) * (b - c));
							if (sc >= 0 && sd < 0)
								segs.pb({sa / (sa - sb), 1});
							else if (sc < 0 && sd >= 0)
								segs.pb({sa / (sa - sb), -1});
						}
					}
			sort(segs.begin(), segs.end());
			db pre = min(max(segs[0].fir, 0.0), 1.0);
			db cur, sum = 0;
			ll cnt = segs[0].sec;
			for (size_t j = 1; j < segs.size(); ++j) {
				cur = min(max(segs[j].fir, 0.0), 1.0);
				if (!cnt)
					sum += cur - pre;
				cnt += segs[j].sec;
				pre = cur;
			}
			ret += im(conj(a) * b) * sum;
		}
	ret = abs(ret) * 0.5;
	return ret;
}
} // namespace lovelive

P randPt(ll lim) { return P(randRange(-lim, lim), randRange(-lim, lim)); }

P rndUlp(ll lim, ll ulps = 5) { return P(randNearIntUlps(lim, ulps), randNearIntUlps(lim, ulps)); }

P rndEps(ll lim, double eps) { return P(randNearIntEps(lim, eps), randNearIntEps(lim, eps)); }

void testRandom(ll n, ll numPts = 10, ll lim = 5, bool brute = false) {
	vector<vector<P>> polygons;
	for (ll i = 0; i < n; i++) {
		vector<P> pts;
		ll k = randIncl(3, numPts);
		for (ll j = 0; j < k; j++) {
			pts.pb(randPt(lim)); // rndEps(lim, 1e-10));
		}
		polygons.pb(genPolygon(pts));
		if (polygonArea2(polygons.back()) < 0) {
			reverse(ALL(polygons.back()));
		}
	}
	auto val1 = polyUnion(polygons);
	vector<vector<blackhorse::pt>> polygons2;
	for (auto i : polygons) {
		vector<blackhorse::pt> t;
		for (auto j : i)
			t.pb({j.x, j.y});
		polygons2.pb(t);
	}
	vector<vector<lovelive::cpoi>> polygons3;
	for (auto i : polygons) {
		vector<lovelive::cpoi> t;
		for (auto j : i)
			t.pb({j.x, j.y});
		polygons3.pb(t);
	}
	auto val3 = blackhorse::polygon_union(polygons2.data(), SZ(polygons2));
	auto val4 = lovelive::polygon_union(polygons3.data(), SZ(polygons3));
	if (abs(val1 - val3) > 1e-8 || abs(val1 - val4) > 1e-8) {
		fore(i, 0, n) {
			for (auto &x : polygons[i]) {
				cout << x << ' ';
			}
			cout << endl;
		}
		abort();
	}
}

int main() {
	// ll s = (ll)time(0);
	ll s = 1;
	// cout << "seed " << s << endl;
	srand(s);
	for (ll i = 0; i < 100; i++) {
		testRandom(2, 5, 5);
	}
	for (ll i = 0; i < 100; i++) {
		testRandom(2, 10, 2);
	}
	for (ll i = 0; i < 50; i++) {
		testRandom(5, 100, 5);
	}
	cout << "Tests passed!" << endl;
}
