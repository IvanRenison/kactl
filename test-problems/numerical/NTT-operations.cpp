// Problem: https://codeforces.com/group/j1UosVRZar/contest/287404/problem/E
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

Poly add(const Poly& p, const Poly& q) { // O(n)
	Poly res(max(SZ(p), SZ(q)));
	fore(i, 0, SZ(p)) res[i] += p[i];
	fore(i, 0, SZ(q)) res[i] += q[i];
	for (ll& x : res) x %= mod;
	while (!res.empty() && !res.back()) res.pop_back();
	return res;
}
Poly derivate(const Poly& p) { // O(n)
	Poly res(max(0ll, SZ(p)-1));
	fore(i, 1, SZ(p)) res[i-1] = (i * p[i]) % mod;
	return res;
}
Poly integrate(const Poly& p) { // O(n)
	Poly ans(SZ(p) + 1);
	fore(i, 0, SZ(p)) ans[i+1] = (p[i] * inv(i+1)) % mod;
	return ans;
}

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
pair<Poly,Poly> div(const Poly& a, const Poly& b) {
	ll m = SZ(a), n = SZ(b);   // O(n log(n)), returns {res, rem}
	if (m < n) return {{}, a}; // if min(m-n,n) < 750 it may be
	Poly ap = a, bp = b;       // faster to use cuadratic version
	reverse(ALL(ap)), reverse(ALL(bp));
	Poly q = conv(ap, inv(bp, m - n + 1));
	q.resize(SZ(q) + m - n - SZ(q) + 1, 0), reverse(ALL(q));
	Poly bq = conv(b, q);
	fore(i, 0, SZ(bq)) if (bq[i]) bq[i] = mod - bq[i];
	return {q, add(a, bq)};
}

vector<Poly> filltree(vi& x) {
	ll k = SZ(x);
	vector<Poly> tr(2*k);
	fore(i, k, 2*k) tr[i] = {(mod - x[i - k]) % mod, 1};
	for (ll i = k; --i;) tr[i] = conv(tr[2*i], tr[2*i+1]);
	return tr;
}
vi evaluate(Poly& a, vi& x) { // O(n log(n)^2)
	ll k = SZ(x);               // Evaluate a in all points of x
	if (!SZ(a)) return vi(k);
	vector<Poly> tr = filltree(x), ans(2*k);
	ans[1] = div(a, tr[1]).snd;
	fore(i, 2, 2*k) ans[i] = div(ans[i/2], tr[i]).snd;
	vi r(k);
	fore(i, 0, k) if (SZ(ans[i+k])) r[i] = ans[i+k][0];
	return r;
}
Poly interpolate(vi& x, vi& y) { // O(n log(n)^2)
	vector<Poly> tr = filltree(x);
	Poly p = derivate(tr[1]);
	ll k = SZ(y);
	vi d = evaluate(p, x); // pass tr here for a speed up
	vector<Poly> intr(2*k);
	fore(i, k, 2*k) intr[i] = {(y[i-k] * inv(d[i-k])) % mod};
	for (ll i = k; --i;) intr[i] = add(
		conv(tr[2*i], intr[2*i+1]), conv(tr[2*i+1], intr[2*i]));
	return intr[1];
}
/// END content

int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll m, k;
	cin >> m >> k;
	vi ns(m);
	for (ll& n : ns) {
		cin >> n;
	}

	vi xs(k + 2), ys(k + 2);
	ll val = 0;
	fore(i, 1, k + 2) {
		val = (val + modpow(i, k)) % mod;
		xs[i] = i, ys[i] = val;
	}

	Poly p = interpolate(xs, ys);

	vi ans = evaluate(p, ns);

	for (ll a : ans) {
		cout << a << ' ';
	}
	cout << '\n';
}
