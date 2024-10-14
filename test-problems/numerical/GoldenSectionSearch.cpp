// Problem: https://codeforces.com/gym/104875
// Status: Accepted
#include <bits/stdc++.h>
using namespace std;

#define fst first
#define snd second
#define pb push_back
#define fore(i, a, b) for (ll i = a, gmat = b; i < gmat; i++)
#define ALL(x) begin(x), end(x)
#define SZ(x) (ll)(x).size()
#define mset(a, v) memset((a), (v), sizeof(a))
typedef long long ll;
typedef pair<ll, ll> ii;
typedef vector<ll> vi;

/// content/numerical/GoldenSectionSearch.h
double gss(double a, double b, auto f) {
	double r = (sqrt(5)-1)/2, x1 = b - r*(b-a), x2 = a + r*(b-a);
	double f1 = f(x1), f2 = f(x2), eps = 1e-7;
	while (b-a > eps)
		if (f1 < f2) // change to > to find maximum
			b = x2, x2 = x1, f2 = f1, f1 = f(x1 = b - r*(b-a));
		else
			a = x1, x1 = x2, f1 = f2, f2 = f(x2 = a + r*(b-a));
	return a;
}
/// END content

int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll h, r, da, dw;
	cin >> h >> r >> da >> dw;

	auto cm = [&](double t) -> double { // Center of mass with t water
		double acm = t + ((double)h - t) / 2.; // Center of mass of air
		double wcm = t / 2.; // Center of mass of water
		double am = (double)da * ((double)h - t); // Mass of air
		double wm = (double)dw * t; // Mass of water
		return (am * acm + wm * wcm) / (am + wm);
	};

	cout << setprecision(10) << gss(0, h, cm) << '\n';
}
