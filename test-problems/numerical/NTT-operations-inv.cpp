// Problem: https://judge.yosupo.jp/problem/inv_of_formal_power_series
// Status: AC
// Submission: https://judge.yosupo.jp/submission/241584
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

/// content/number-theory/ModPow.h
/// START diff
const ll mod = (119 << 23) + 1; // faster if const
/// END diff
ll modpow(ll b, ll e) {
	ll ans = 1;
	for (; e; b = b * b % mod, e /= 2)
		if (e & 1) ans = ans * b % mod;
	return ans;
}
/// END content

/// content/numerical/NumberTheoreticTransform.h
/// START diff
const ll /* mod = (119 << 23) + 1, */ root = 62; // = 998244353
/// END diff
// For p < 2^30 there is also e.g. 5 << 25, 7 << 26, 479 << 21
// and 483 << 21 (same root). The last two are > 10^9.
void ntt(vi &a) {
	ll n = SZ(a), L = 63 - __builtin_clzll(n);
	static vi rt(2, 1);
	for (static ll k = 2, s = 2; k < n; k *= 2, s++) {
		rt.resize(n);
		ll z[] = {1, modpow(root, mod >> s)};
		fore(i,k,2*k) rt[i] = rt[i / 2] * z[i & 1] % mod;
	}
	vi rev(n);
	fore(i,0,n) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
	fore(i,0,n) if (i < rev[i]) swap(a[i], a[rev[i]]);
	for (ll k = 1; k < n; k *= 2)
		for (ll i = 0; i < n; i += 2 * k) fore(j,0,k) {
			ll z = rt[j + k] * a[i + j + k] % mod, &ai = a[i + j];
			a[i + j + k] = ai - z + (z > ai ? mod : 0);
			ai += (ai + z >= mod ? z - mod : z);
		}
}
vi conv(const vi &a, const vi &b) {
	if (a.empty() || b.empty()) return {};
	ll s = SZ(a) + SZ(b) - 1, B = 64 - __builtin_clzll(s),
	    n = 1 << B;
	ll inv = modpow(n, mod - 2);
	vi L(a), R(b), out(n);
	L.resize(n), R.resize(n);
	ntt(L), ntt(R);
	fore(i,0,n)
		out[-i & (n - 1)] = (ll)L[i] * R[i] % mod * inv % mod;
	ntt(out);
	return {out.begin(), out.begin() + s};
}
/// END content

/// content/number-theory/FastInverse.h
/// START diff
/// constexpr ll mod = 1e9 + 7;
/// END diff
constexpr ll k = bit_width((unsigned long long)(mod - 2));
ll inv(ll a) {
	ll r = 1;
#pragma GCC unroll(k)
	fore(l, 0, k) {
		if ((mod - 2) >> l & 1) r = r * a % mod;
		a = a * a % mod;
	}
	return r;
}
/// END content

/// content/numerical/NTT-operations.h
typedef vi Poly;
/// START diff
/// END diff
Poly takeMod(Poly p, ll n) { // O(n)
	p.resize(min(SZ(p), n));   // p % (x^n)
	while (!p.empty() && !p.back()) p.pop_back();
	return p;
}

Poly inv(const Poly& p, ll d) { // O(n log(n))
	Poly res = {inv(p[0])};       // first d terms of 1/p
	ll sz = 1;
	while (sz < d) {
		sz *= 2;
		Poly pre(p.begin(), p.begin() + min(SZ(p), sz));
		Poly cur = conv(res, pre);
		fore(i, 0, SZ(cur)) if (cur[i]) cur[i] = mod - cur[i];
		cur[0] = cur[0] + 2;
		res = takeMod(conv(res, cur), sz);
	}
	res.resize(d);
	return res;
}
/// START diff
/// END diff
/// END content

int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll N;
	cin >> N;
	vi as(N);
	for (ll& a : as) {
		cin >> a;
	}
	while (as.size() && !as.back()) as.pop_back();

	Poly ans = inv(as, N);

	for (ll b : ans) {
		cout << b << ' ';
	}
	cout << '\n';
}
