// Problem: https://codeforces.com/contest/242/problem/E
// Status: Time limit exceeded on test 16
// Submission:
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

/// content/data-structures/IterativeLazySegmentTree.h
struct Tree {
	/// START diff
	struct T {
		vi cnts;
		ll sz;
	}; // data type
	typedef ll L; // lazy type
	const T tneut = T{vi(20), 0};		 // neutral elements
	const L lneut = 0;
	T f (T a, T b) {
		T ans = a;
		ans.sz += b.sz;
		fore(i, 0, 20) {
			ans.cnts[i] += b.cnts[i];
		}
		return ans;
	} // (any associative fn)
	T apply (T a, L b) {
		T ans = a;
		fore(i, 0, 20) {
			if (b & (1 << i)) {
				ans.cnts[i] = a.sz - ans.cnts[i];
			}
		}
		return ans;
	} // Apply lazy
	L comb(L a, L b) {
		return a ^ b;
	} // Combine lazy
	/// END diff

	ll n;
	vector<T> s;
	vector<L> d;

	Tree(ll n = 0) : n(n), s(2*n, tneut), d(n, lneut) {}
	Tree(vector<T>& vals) : n(SZ(vals)), s(2*n), d(n, lneut) {
		copy(ALL(vals), begin(s) + n);
		for (ll i = n - 1; i > 0; i--) s[i] = f(s[2*i], s[2*i+1]);
	}

	void apply_(ll p, L value) {
		s[p] = apply(s[p], value);
		if (p < n) d[p] = comb(d[p], value);
	}

	void build(ll p) {
		while (p > 1)
			p /= 2, s[p] = apply(f(s[2*p], s[2*p+1]), d[p]);
	}

	void push(ll p) {
		for (ll s = 63 - __builtin_clzll(n); s > 0; s--) {
			ll i = p >> s;
			apply_(2*i, d[i]);
			apply_(2*i+1, d[i]);
			d[i] = lneut;
		}
	}

	void upd(ll l, ll r, L value) {
		l += n, r += n;
		push(l);
		push(r - 1);
		ll l0 = l, r0 = r;
		for (; l < r; l /= 2, r /= 2) {
			if (l&1) apply_(l++, value);
			if (r&1) apply_(--r, value);
		}
		build(l0);
		build(r0 - 1);
	}

	T query(ll l, ll r) {
		l += n, r += n;
		push(l);
		push(r - 1);
		T ans = tneut;
		for (; l < r; l /= 2, r /= 2) {
			if (l&1) ans = f(ans, s[l++]);
			if (r&1) ans = f(s[--r], ans);
		}
		return ans;
	}
};
/// END content


int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll n;
	cin >> n;

	vector<Tree::T> vals(n, Tree::T{vi(20), 1});
	for (Tree::T& v : vals) {
		ll a;
		cin >> a;

		fore(i, 0, 20) {
			if (a & (1 << i)) {
				v.cnts[i]++;
			}
		}
	}

	Tree t(vals);

	ll q;
	cin >> q;

	while (q--) {
		ll ty, l, r;
		cin >> ty >> l >> r;
		l--;
		if (ty == 1) {
			Tree::T ans_T = t.query(l, r);
			ll ans = 0;
			fore(i, 0, 20) {
				ans += ans_T.cnts[i] * (1 << i);
			}

			cout << ans << '\n';
		} else {
			ll x;
			cin >> x;
			t.upd(l, r, x);
		}
	}

}
