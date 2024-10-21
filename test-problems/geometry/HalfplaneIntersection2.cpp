// Problem: https://codeforces.com/gym/101309/problem/J
// Status: Accepted
#include <bits/stdc++.h>
using namespace std;

#define fst first
#define snd second
#define pb push_back
#define fore(i, a, b) for (ll i = a, gmat = b; i < gmat; i++)
#define ALL(x) x.begin(), x.end()
#define SZ(x) (ll)(x).size()
#define mset(a, v) memset((a), (v), sizeof(a))
typedef long long ll;
typedef pair<ll, ll> ii;
typedef vector<ll> vi;

/// content/geometry/Point.h
template <class T> ll sgn(T x) { return (x > 0) - (x < 0); }
template<class T>
struct Point {
	typedef Point P;
	T x, y;
	explicit Point(T x=0, T y=0) : x(x), y(y) {}
	bool operator<(P p) const { return tie(x,y) < tie(p.x,p.y); }
	bool operator==(P p) const { return tie(x,y)==tie(p.x,p.y); }
	P operator+(P p) const { return P(x+p.x, y+p.y); }
	P operator-(P p) const { return P(x-p.x, y-p.y); }
	P operator*(T d) const { return P(x*d, y*d); }
	P operator/(T d) const { return P(x/d, y/d); }
	T dot(P p) const { return x*p.x + y*p.y; }
	T cross(P p) const { return x*p.y - y*p.x; }
	T cross(P a, P b) const { return (a-*this).cross(b-*this); }
	T dist2() const { return x*x + y*y; }
	double dist() const { return sqrt((double)dist2()); }
	// angle to x-axis in interval [-pi, pi]
	double angle() const { return atan2(y, x); }
	P unit() const { return *this/dist(); } // makes dist()=1
	P perp() const { return P(-y, x); } // rotates +90 degrees
	P normal() const { return perp().unit(); }
	// returns point rotated 'a' radians ccw around the origin
	P rotate(double a) const {
		return P(x*cos(a)-y*sin(a),x*sin(a)+y*cos(a)); }
	friend ostream& operator<<(ostream& os, P p) {
		return os << "(" << p.x << "," << p.y << ")"; }
};
/// END content

/// content/geometry/sideOf.h
template<class P>
ll sideOf(P s, P e, P p) { return sgn(s.cross(e, p)); }

template<class P>
ll sideOf(const P& s, const P& e, const P& p, double eps) {
	auto a = (e-s).cross(p-s);
	double l = (e-s).dist()*eps;
	return (a > l) - (a < -l);
}
/// END content

/// content/geometry/lineIntersection.h
template<class P>
pair<ll, P> lineInter(P s1, P e1, P s2, P e2) {
	auto d = (e1 - s1).cross(e2 - s2);
	if (d == 0) // if parallel
		return {-(s1.cross(e1, s2) == 0), P(0, 0)};
	auto p = s2.cross(e1, e2), q = s2.cross(e2, s1);
	return {1, (s1 * p + e1 * q) / d};
}
/// END content

/// content/geometry/HalfplaneIntersection.h
typedef Point<double> P;
struct Line {
	P p, q;
	double a;
	Line() {}
	Line(P p, P q) : p(p), q(q), a((q - p).angle()) {}
	bool operator<(Line o) const { return a < o.a; }
};
#define L(a) a.p, a.q
#define PQ(a) (a.q - a.p)

vector<P> halfPlaneIntersection(vector<Line> v) {
	sort(ALL(v));
	ll n = SZ(v), q = 1, h = 0;
	const double eps = 1e-9;
	vector<Line> c(n+2);
	#define I(j, k) lineInter(L(c[j]), L(c[k])).snd
	fore(i, 0, n) {
		while (q < h && sideOf(L(v[i]), I(h, h-1), eps) < 0) h--;
		while (q < h && sideOf(L(v[i]), I(q, q+1), eps) < 0) q++;
		c[++h] = v[i];
		if (q < h && abs(PQ(c[h]).cross(PQ(c[h-1]))) < eps) {
			if (PQ(c[h]).dot(PQ(c[h-1])) <= 0) return {};
			if (sideOf(L(v[i]), c[--h].p, eps) < 0) c[h] = v[i];
		}
	}
	while (q < h - 1 && sideOf(L(c[q]), I(h, h-1), eps) < 0) h--;
	while (q < h - 1 && sideOf(L(c[h]), I(q, q+1), eps) < 0) q++;
	if (h - q <= 1) return {};
	c[++h] = c[q];
	vector<P> s;
	fore(i, q, h) s.pb(I(i, i+1));
	return s;
}
/// END content

/// content/geometry/PolygonArea.h
template<class T>
T polygonArea2(vector<Point<T>>& v) {
	T a = v.back().cross(v[0]);
	fore(i,0,SZ(v)-1) a += v[i].cross(v[i+1]);
	return a;
}
/// END content

const double eps = 1e-11;

ll solve(vector<P>& points) {
	ll n = SZ(points);

	ll l = 0, r = n - 2;
	while (l + 1 < r) {
		ll m = (l + r) / 2;

		vector<Line> lines(n);
		fore(i, 0, n) {
			lines[i] = {points[(i + m + 1) % n], points[i]};
		}

		vector<P> poly = halfPlaneIntersection(lines);

		double area = poly.size() <= 2 ? 0 : polygonArea2(poly);

		if (area <= eps) {
			r = m;
		} else {
			l = m;
		}
	}

	return r;
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
#ifdef ONLINE_JUDGE
	freopen("jungle.in","r",stdin);
	freopen("jungle.out","w",stdout);
#endif

	ll n;
	cin >> n;
	vector<P> points(n);
	for (auto& [x, y] : points) {
		ll X, Y;
		cin >> X >> Y;
		x = X, y = Y;
	}

	auto ans = solve(points);
	cout << ans << '\n';
}
