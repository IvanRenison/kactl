// Problem: https://cses.fi/problemset/task/2072
// Status: ACCEPTED
// Submission: https://cses.fi/problemset/result/11044522/
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

/// content/data-structures/Treap.h
/// START diff
// We are not interested  in queries
typedef char T; typedef ll L; // T: data type, L: lazy type
const T tneut = 0; const L lneut = 0; // neutrals
T f(T a, T b) { return a; } // associative
T apply(T v, L l, ll len) { // new st according to lazy
	return v; }
L comb(L a, L b) { return a; } // cumulative effect of lazy
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

string getVals(Node* t) {
	if (!t) return "";
	string ans = getVals(t->l);
	ans += t->val;
	ans += getVals(t->r);
	return ans;
}

int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll n, m;
	cin >> n >> m;
	string s;
	cin >> s;

	vector<Node> nodes(n);
	fore(i, 0, n) nodes[i] = Node(s[i]);

	Node* t = &nodes[0];
	fore(i, 1, n) {
		t->merge(&nodes[i]);
	}

	while (m--) {
		ll a, b;
		cin >> a >> b;
		a--;

		if (b == n) continue;

		Node* r = t->split(b);
		if (a == 0) {
			r->merge(t);
			t = r;
		} else {
			Node* m = t->split(a);
			t->merge(r);
			t->merge(m);
		}
	}

	string ans = getVals(t);
	cout << ans << '\n';
}
