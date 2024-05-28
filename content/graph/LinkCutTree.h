/**
 * Author: Iván Renison
 * Date: 2024-05-26
 * Source: notebook el vasito
 * Description: Represents a forest of rooted trees. You can add and remove
 * edges (as long as the result is still a forest), make path queries ans path updates
 * Time: All operations take amortized O(\log N).
 * Status: Stress-tested a bit for N <= 20
 */
#pragma once

typedef ll T; // T: data type, L: lazy type
typedef ll L;
const L lneut = 0;
const L tneut = 0;
T f(T a, T b) { return T{a + b}; } // operation
// new st according to lazy
T apply(T v, L l, ll len) { return T{v + l * len}; }
// cumulative effect of lazy
L comb(L a, L b) { return L{a + b}; }
//mostly generic
struct LinkCutTree {
	struct Node {
		ll sz_;
		T nVal, tVal;
		L d;
		bool rev;
		ll c[2], p;
		Node(T v = tneut) : sz_(1), nVal(v), tVal(v), d(lneut), rev(0), p(-1) {
			c[0] = c[1] = -1;
		}
	};
	ll n;
	vector<Node> nodes;

	LinkCutTree(ll n) : n(n), nodes(n) {
		fore(i, 0, n) {
			nodes[i] = Node(tneut);
		}
	}
	LinkCutTree(vector<T>& vals) : n(vals.size()), nodes(n) {
		fore(i, 0, n) {
			nodes[i] = Node(vals[i]);
		}
	}

	bool isRoot(ll u) {
		return nodes[u].p == -1 || (nodes[nodes[u].p].c[0] != u && nodes[nodes[u].p].c[1] != u);
	}
	void push(ll u) {
		if (nodes[u].rev) {
			nodes[u].rev=0;
			swap(nodes[u].c[0], nodes[u].c[1]);
			fore(x, 0, 2) if (nodes[u].c[x] != -1) nodes[nodes[u].c[x]].rev ^= 1;
		}
		nodes[u].nVal = apply(nodes[u].nVal, nodes[u].d, 1);
		nodes[u].tVal = apply(nodes[u].tVal, nodes[u].d, nodes[u].sz_);
		fore(x, 0, 2) if(nodes[u].c[x] != -1)
			nodes[nodes[u].c[x]].d = comb(nodes[nodes[u].c[x]].d, nodes[u].d);
		nodes[u].d=lneut;
	}
	T getPV(ll u) {
		return u == -1 ? tneut : apply(nodes[u].tVal, nodes[u].d, nodes[u].sz_);
	}
	ll getSz(ll u) {
		return u == -1 ? 0 : nodes[u].sz_;
	}
	void upd(ll u) {
		nodes[u].tVal = f(
			f(getPV(nodes[u].c[0]), apply(nodes[u].nVal, nodes[u].d, 1)),
			getPV(nodes[u].c[1]));
		nodes[u].sz_ = 1 + getSz(nodes[u].c[0]) + getSz(nodes[u].c[1]);
	}

	void conn(ll c, ll p, ll il) {
		if (c != -1) nodes[c].p = p;
		if (il >= 0) nodes[p].c[!il] = c;
	}
	void rotate(ll u) {
		ll p = nodes[u].p, g = nodes[p].p;
		bool gCh = isRoot(p), isl = u == nodes[p].c[0];
		conn(nodes[u].c[isl], p, isl);
		conn(p, u, !isl);
		conn(u, g, gCh ? -1 : (p == nodes[g].c[0]));
		upd(p);
	}
	void spa(ll u) { // splay
		while (!isRoot(u)) {
			ll p = nodes[u].p, g = nodes[p].p;
			if (!isRoot(p)) push(g);
			push(p), push(u);
			if (!isRoot(p)) rotate((u == nodes[p].c[0]) == (p == nodes[g].c[0]) ? p : u);
			rotate(u);
		}
		push(u), upd(u);
	}
	ll lift_rec(ll u, ll t) {
		if(u == -1) return 0;
		ll s = getSz(nodes[u].c[0]);
		if(t == s) {
			spa(u);
			return u;
		}
		if(t < s) return lift_rec(nodes[u].c[0], t);
		return lift_rec(nodes[u].c[1], t - s - 1);
	}
	ll exv(ll u){ // expose
		ll last = -1;
		for (ll v = u; v != -1; v = nodes[v].p)
			spa(v), nodes[v].c[0] = last, upd(v), last = v;
		spa(u);
		return last;
	}

	void mkR(ll u){ // makeRoot
		exv(u);
		nodes[u].rev ^= 1;
	}
	ll getR(ll u){
		exv(u);
		while (nodes[u].c[1]) u = nodes[u].c[1];
		spa(u);
		return u;
	}
	ll lca(ll u, ll v) {
		exv(u);
		return exv(v);
	}
	bool connected(ll u, ll v) {
		exv(u), exv(v);
		return u == v ? true : nodes[u].p != -1;
	}
	void link(ll u, ll v) {
		mkR(u);
		nodes[u].p = v;
	}
	void cut(ll u, ll v) {
		mkR(u), exv(v);
		nodes[nodes[v].c[1]].p = -1, nodes[v].c[1] = -1;
	}
	ll father(ll u){
		exv(u);
		ll v = nodes[u].c[1];
		if (v == -1) return -1;
		while (nodes[v].c[0] != -1) v = nodes[v].c[0];
		return v;
	}
	void cut(ll u) { // cuts x from father keeping tree root
		exv(u);
		nodes[u].p = -1;
	}
	T query(ll u, ll v) {
		mkR(u), exv(v);
		return getPV(v);
	}
	void modify(ll u, ll v, L d) {
		mkR(u), exv(v);
		nodes[v].d = comb(nodes[v].d, d);
	}
	ll depth(ll u) { // distance from x to its tree root
		exv(u);
		return nodes[u].sz_ - 1;
	}
	ll lift(ll u, ll t) { // t-th ancestor of x (lift(x,1) is x's father)
		exv(u);
		return lift_rec(u, t);
	}
};
