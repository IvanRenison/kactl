/**
 * Author: someone on Codeforces and then modified by Iván Renison
 * Date: 2017-03-14
 * Source: folklore
 * Description: A short self-balancing tree. It acts as a
 *  sequential container with log-time splits, joins, queries ans updates.
 *  Can also support reversals with the commented REVERSE lines and getting the
 * position of a node with the PARENT lines.
 * Time: $O(\log N)$
 * Status: stress-tested and problem tested
 */
#pragma once

typedef ll T; typedef ll L; // T: data type, L: lazy type
const T tneut = 0; const L lneut = 0; // neutrals
T f(T a, T b) { return a + b; } // operation
// new st according to lazy
T apply(T v, L l, ll len) { return v + l * len; }
// cumulative effect of lazy
L comb(L a, L b) { return a + b; }

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
	// ll pos() { // In witch position I am // PARENT
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
