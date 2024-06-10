/**
 * Author: Iván Renison
 * Date: 2024-05-26
 * Source: notebook el vasito
 * Description: Represents a forest of rooted trees with nodes \textbf{indexed from one}.
 * You can add and remove edges (as long as the result is still a forest),
 * make path queries and path updates
 * Time: All operations take amortized O(\log N).
 * Status: Stress-tested a bit
 */
#pragma once

struct LinkCutTree {
	typedef ll T; typedef ll L; // T: data type, L: lazy type
	static constexpr L lneut = 0; static constexpr L tneut = 0;
	T f(T a, T b) { return a + b; } // operation
	// new st according to lazy
	T apply(T v, L l, ll len) { return v + l * len; }
	// cumulative effect of lazy
	L comb(L a, L b) { return a + b; }

	struct Node {
		ll sz_; bool rev;
		T val, t; L d;
		array<ll, 2> c; ll p;
		Node(T v = tneut) : sz_(1), val(v), t(v), d(lneut) {}
	};
	vector<Node> nods;
	LinkCutTree(ll n) : nods(n + 1) {
		fore(i, 0, n) nods[i + 1] = Node(tneut);
		nods[0] = Node(tneut), nods[0].sz_ = 0;
	}
	LinkCutTree(vector<T>& a) : nods(SZ(a) + 1) {
		fore(i, 0, SZ(a)) nods[i + 1] = Node(a[i]);
		nods[0] = Node(tneut), nods[0].sz_ = 0;
	}
	bool isRoot(ll u) {
		Node N = nods[nods[u].p];
		return N.c[0] != u && N.c[1] != u;
	}
	void push(ll u) {
		Node& N = nods[u];
		if (N.rev) {
			N.rev = 0, swap(N.c[0], N.c[1]);
			fore(x, 0, 2) if (N.c[x]) nods[N.c[x]].rev ^= 1;
		}
		N.val = apply(N.val, N.d, 1);
		N.t = apply(N.t, N.d, N.sz_);
		fore(x, 0, 2) if (N.c[x])
			nods[N.c[x]].d = comb(nods[N.c[x]].d, N.d);
		N.d = lneut;
	}
	T get(ll u) {
		return apply(nods[u].t, nods[u].d, nods[u].sz_);
	}
	void calc(ll u) {
		Node& N = nods[u];
		N.t = f(f(get(N.c[0]), apply(N.val, N.d, 1)), get(N.c[1]));
		N.sz_ = 1 + nods[N.c[0]].sz_ + nods[N.c[1]].sz_;
	}
	void conn(ll c, ll p, ll il) {
		if (c) nods[c].p = p;
		if (il >= 0) nods[p].c[!il] = c;
	}
	void rotate(ll u) {
		ll p = nods[u].p, g = nods[p].p;
		bool gCh = isRoot(p), isl = u == nods[p].c[0];
		conn(nods[u].c[isl], p, isl);
		conn(p, u, !isl);
		conn(u, g, gCh ? -1 : (p == nods[g].c[0]));
		calc(p);
	}
	void spa(ll u) { // splay
		while (!isRoot(u)) {
			ll p = nods[u].p, g = nods[p].p;
			if (!isRoot(p)) push(g);
			push(p), push(u);
			if (!isRoot(p))
				rotate((u==nods[p].c[0]) ^ (p==nods[g].c[0]) ? u : p);
			rotate(u);
		}
		push(u), calc(u);
	}
	ll lift_rec(ll u, ll t) {
		if (!u) return 0;
		Node N = nods[u];
		ll s = nods[N.c[0]].sz_;
		if (t == s) return spa(u), u;
		if (t < s) return lift_rec(N.c[0], t);
		return lift_rec(N.c[1], t - s - 1);
	}
	ll exv(ll u){ // expose
		ll last = 0;
		for (ll v = u; v; v = nods[v].p)
			spa(v), nods[v].c[0] = last, calc(v), last = v;
		spa(u);
		return last;
	}

	void mkR(ll u){ // make root of its tree
		exv(u), nods[u].rev ^= 1;
	}
	ll getR(ll u){
		exv(u);
		while (nods[u].c[1]) u = nods[u].c[1];
		spa(u);
		return u;
	}
	ll lca(ll u, ll v) { // least common ancestor
		exv(u);
		return exv(v);
	}
	bool connected(ll u, ll v) { // are u and v in the same tree
		exv(u), exv(v);
		return u == v || nods[u].p != 0;
	}
	void link(ll u, ll v) { // add edge between u and v
		mkR(u), nods[u].p = v;
	}
	void cut(ll u, ll v) { // remove edge u v
		mkR(u), exv(v);
		nods[nods[v].c[1]].p = 0, nods[v].c[1] = 0;
	}
	ll father(ll u) { // father of u, 0 if u is root
		exv(u), u = nods[u].c[1];
		while (nods[u].c[0]) u = nods[u].c[0];
		return u;
	}
	void cut(ll u) { // cuts x from father keeping tree root
		exv(u), nods[u].p = 0;
	}
	T query(ll u, ll v) { // query on path from u to v
		mkR(u), exv(v);
		return get(v);
	}
	void upd(ll u, ll v, L d) { // modify path from u to v
		mkR(u), exv(v), nods[v].d = comb(nods[v].d, d);
	}
	ll depth(ll u) { // distance from x to its tree root
		exv(u);
		return nods[u].sz_ - 1;
	}
	ll lift(ll u, ll t) {//t ancestor of x, lift(x,1) is x father
		exv(u);
		return lift_rec(u, t);
	}
};
