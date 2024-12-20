// Problem: https://codeforces.com/contest/622/problem/F
// Status: Accepted
// Submission: https://codeforces.com/contest/622/submission/286770593
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

/// content/number-theory/euclid.h
ll euclid(ll a, ll b, ll &x, ll &y) {
	if (!b) return x = 1, y = 0, a;
	ll d = euclid(b, a % b, y, x);
	return y -= a/b * x, d;
}
/// END content

/// content/number-theory/ModularArithmetic.h
/// START diff
const ll mod = 1e9 + 7;
/// END diff
struct Mod {
	ll x;
	Mod(ll xx) : x(xx) {}
	Mod operator+(Mod b) { return Mod((x + b.x) % mod); }
	Mod operator-(Mod b) { return Mod((x - b.x + mod) % mod); }
	Mod operator*(Mod b) { return Mod((x * b.x) % mod); }
	Mod operator/(Mod b) { return *this * invert(b); }
	Mod invert(Mod a) {
		return a ^ (mod - 2);
		// ll x, y, g = euclid(a.x, mod, x, y);
		// assert(g == 1); return Mod((x + mod) % mod);
	}
	Mod operator^(ll e) {
		Mod ans(1);
		for (Mod b = *this; e; b = b * b, e >>= 1)
			if (e & 1) ans = ans * b;
		return ans;
	}
};
/// END content

/// content/numerical/PolyInterpolatePuntual.h
Mod interpolate(vector<Mod>& y, Mod x) {
	ll n = SZ(y);
	static vector<Mod> fi {1}; // Inverses of factorials
	while (SZ(fi) < n) fi.pb(fi.back() / SZ(fi));
	if (x.x < n) return y[x.x];
	Mod p = 1;
	fore(i, 0, n) p = p * (x - i);
	Mod ans = 0;
	fore(i, 0, n) {
		Mod t = y[i] * p / (x-i) * fi[i] * fi[n-1-i];
		if ((n-i) % 2 == 0) t = t * (mod - 1);
		ans = ans + t;
	}
	return ans;
}
/// END content

Mod solve(ll n, ll k) {
	vector<Mod> y = {0};
	fore(i, 1, k + 2) {
		y.pb(y.back() + (Mod(i) ^ k));
	}
	Mod ans = interpolate(y, Mod(n));
	return ans;
}

int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll n, k;
	cin >> n >> k;
	Mod ans = solve(n, k);
	cout << ans.x << '\n';
}
