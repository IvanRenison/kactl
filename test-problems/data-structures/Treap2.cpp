// Problem: https://judge.yosupo.jp/problem/range_set_range_composite
// Status: TLE
// Submission: https://judge.yosupo.jp/submission/243655
#include <bits/stdc++.h>
using namespace std;

#pragma GCC optimize("O3")

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

const ll mod = 998244353;

/// content/data-structures/Treap.h
/// START diff
struct T {
	ll a, b;

	T(ll a = 1, ll b = 0) : a(a % mod), b(b % mod) {}
	T operator*(T o) {
		return {a * o.a % mod, (o.a * b + o.b) % mod};
	}
	T operator^(ll e) {
		T ans, x = *this;
		for (; e; e >>= 1, x = x * x) if (e & 1) ans = ans * x;
		return ans;
	}
	ll operator()(ll x) {
		return (a * x + b) % mod;
	}
};
typedef optional<T> L; // T: data type, L: lazy type
// neutrals
const T tneut;
const L lneut;
T f(T a, T b) { return a * b; } // operation
// new st according to lazy
T apply(T v, L l, ll len) {
	if (!l) return v;
	else return *l^len;
}
// cumulative effect of lazy
L comb(L a, L b) {
	if (!b) return a;
	else return b;
}
/// END diff
struct Node {
	Node *l = 0, *r = 0;
	// Node *p = 0; // PARENT
	T val, acc; L lazy = lneut;
	ll y, c = 1;
	// bool rev = false; // REVERSE
	Node(T val = tneut) : val(val), acc(val), y(rand()) {}
	void recalc() {
		c = 1, acc = tneut;
		if (l) l->push(), acc = f(acc, l->acc), c += l->c;
		acc = f(acc, val);
		if (r) r->push(), acc = f(acc, r->acc), c += r->c;
		// if (l) l->p = this; // PARENT
		// if (r) r->p = this;
	}
	void push() {
		// if (rev) { // REVERSE
		// 	swap(l, r), rev = false;
		// 	if (l) l->rev ^= 1; if (r) r->rev ^= 1;
		// }
		val = apply(val, lazy, 1), acc = apply(acc, lazy, c);
		if (l) l->lazy = comb(l->lazy, lazy);
		if (r) r->lazy = comb(r->lazy, lazy);
		lazy = lneut;
	}
	// void pullAll() { // PARENT
	// 	if (p) p->pullAll();
	// 	push();
	// }

	Node* split(ll k) {
		assert(k > 0);
		if (k >= c) return 0;
		push();
		ll cnt = l ? l->c : 0;
		if (k <= cnt) { // "k <= val" for lower_bound(k)
			Node* nl = l->split(k),* ret = l;
			l = nl;
			recalc();
			swap(*this, *ret);
			return ret;
		} else if (k == cnt + 1) { // k == val
			Node* ret = r;
			r = 0;
			recalc();
			return ret;
		} else {
			Node* ret = r->split(k - cnt - 1); // and just "k"
			recalc(), ret->recalc();
			return ret;
		}
	}
	void merge(Node* ri) {
		if (!ri) return;
		push(), ri->push();
		if (y > ri->y) {
			if (r) r->merge(ri);
			else r = ri;
		} else {
			merge(ri->l);
			ri->l = ri;
			swap(*this, *ri);
		}
		recalc();
	}
	// ll pos() { // In which position I am // PARENT
	// 	pullAll();
	// 	ll ans = l ? l->c : 0;
	// 	if (!p) return ans;
	// 	if (p->r == this) return ans + p->pos() + 1;
	// 	else return p->pos() + 1 - (r ? r->c : 0);
	// }
	T query() { // Query full range
		push();
		return acc;
	}
	void upd(L v) { lazy = comb(lazy, v); } // Update full range
	// void reverse() { rev = !rev; } // REVERSE
};
/// END content

void update(Node& n, ll p, ll q, T v) {
	Node* r = n.split(q);
	if (p > 0) {
		Node* m = n.split(p);
		m->upd(v);
		n.merge(m);
	} else {
		n.upd(v);
	}
	n.merge(r);
}

T query(Node& n, ll p, ll q) {
	Node* r = n.split(q);
	T ans;
	if (p > 0) {
		Node* m = n.split(p);
		ans = m->query();
		n.merge(m);
	} else {
		ans = n.query();
	}
	n.merge(r);
	return ans;
}

int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll N, C;
	cin >> N >> C;

	ll a, b;
	cin >> a >> b;
	vector<Node> nodes(N);
	Node& t = nodes[0] = Node(T(a, b));
	fore(i, 1, N) {
		cin >> a >> b;
		nodes[i] = Node(T(a, b));
		t.merge(&nodes[i]);
	}

	fore(_, 0, C) {
		ll ty, l, r;
		cin >> ty >> l >> r;

		if (ty == 0) { // Update
			ll c, d;
			cin >> c >> d;
			update(t, l, r, T(c, d));
		} else { // Query
			ll x;
			cin >> x;
			T ans = query(t, l, r);
			cout << ans(x) << '\n';
		}
	}
}
