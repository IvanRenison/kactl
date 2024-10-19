// Problem: https://codeforces.com/gym/105321/problem/C
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

/// content/geometry/Point3D.h
template<class T> struct Point3D {
	typedef Point3D P;
	typedef const P& R;
	T x, y, z;
	explicit Point3D(T x=0, T y=0, T z=0) : x(x), y(y), z(z) {}
	bool operator<(R p) const {
		return tie(x, y, z) < tie(p.x, p.y, p.z); }
	bool operator==(R p) const {
		return tie(x, y, z) == tie(p.x, p.y, p.z); }
	P operator+(R p) const { return P(x+p.x, y+p.y, z+p.z); }
	P operator-(R p) const { return P(x-p.x, y-p.y, z-p.z); }
	P operator*(T d) const { return P(x*d, y*d, z*d); }
	P operator/(T d) const { return P(x/d, y/d, z/d); }
	T dot(R p) const { return x*p.x + y*p.y + z*p.z; }
	P cross(R p) const {
		return P(y*p.z - z*p.y, z*p.x - x*p.z, x*p.y - y*p.x);
	}
	T dist2() const { return x*x + y*y + z*z; }
	double dist() const { return sqrt((double)dist2()); }
	//Azimuthal angle (longitude) to x-axis in interval [-pi, pi]
	double phi() const { return atan2(y, x); }
	//Zenith angle (latitude) to the z-axis in interval [0, pi]
	double theta() const { return atan2(sqrt(x*x+y*y),z); }
	P unit() const { return *this/(T)dist(); } //makes dist()=1
	//returns unit vector normal to *this and p
	P normal(P p) const { return cross(p).unit(); }
	//returns point rotated 'angle' radians ccw around axis
	P rotate(double angle, P axis) const {
		double s = sin(angle), c = cos(angle); P u = axis.unit();
		return u*dot(u)*(1-c) + (*this)*c - cross(u)*s;
	}
};
/// END content

/// content/geometry/sideOfPlane.h
template<class P>
ll sideOf(P a, P b, P c, P p) {
	ll x = (b-a).cross(c-a).dot(p-a);
	return (x > 0) - (x < 0);
}
/// END content

bool solve(Point3D<ll> cus, Point3D<ll> sun,const vector<Point3D<ll>>& poly) {
	ll N = poly.size();

	fore(i, 0, N) {
		Point3D<ll> p = poly[i], q = poly[(i + 1) % N];
		if (sideOf(p, q, cus, sun) == 1) {
			return true;
		}
	}

	return false;
}

int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll N;
	cin >> N;

	Point3D<ll> cus, sun;
	cin >> cus.x >> cus.y >> cus.z >> sun.x >> sun.y >> sun.z;

	vector<Point3D<ll>> poly(N);
	for (auto& [X, Y, Z] : poly) {
		cin >> X >> Y;
	}

	bool ans = solve(cus, sun, poly);

	if (ans) {
		cout << "S\n";
	} else {
		cout << "N\n";
	}
}
