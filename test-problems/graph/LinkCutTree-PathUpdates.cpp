// Problem: https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum
// Status: AC
// Submission: https://judge.yosupo.jp/submission/246194
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

/// content/graph/LinkCutTree-PathUpdates.h
struct LinkCutTree {
	typedef ll T; typedef ll L; // T: data type, L: lazy type
	static constexpr T tneut = 0; static constexpr L lneut = 0;
	T f(T a, T b) { return a + b; } // associative & commutative
	T apply(T v, L l, ll len) { // new st according to lazy
		return v + l * len; }
	L comb(L a, L b) { return a + b; }//cumulative effect of lazy
	struct Node {
		ll s = 1; bool rev = 0;
		T val, t; L d = lneut;
		array<ll, 2> c = {0, 0}; ll p = 0;
		Node(T v = tneut) : val(v), t(v) {}
	};
	vector<Node> nods;
	LinkCutTree(ll n) : nods(n + 1) {
		nods[0].s = 0;
	}
	LinkCutTree(vector<T>& a) : nods(SZ(a) + 1) {
		fore(i, 0, SZ(a)) nods[i + 1] = Node(a[i]);
		nods[0].s = 0;
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
		N.t = apply(N.t, N.d, N.s);
		fore(x, 0, 2) if (N.c[x])
			nods[N.c[x]].d = comb(nods[N.c[x]].d, N.d);
		N.d = lneut;
	}
	T get(ll u) {
		return apply(nods[u].t, nods[u].d, nods[u].s);
	}
	void calc(ll u) {
		Node& N = nods[u];
		N.t = f(f(get(N.c[0]), apply(N.val, N.d, 1)), get(N.c[1]));
		N.s = 1 + nods[N.c[0]].s + nods[N.c[1]].s;
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
		ll s = nods[N.c[0]].s;
		if (t == s) return spa(u), u;
		if (t < s) return lift_rec(N.c[0], t);
		return lift_rec(N.c[1], t - s - 1);
	}
	ll exv(ll u) { // expose
		ll last = 0;
		for (ll v = u; v; v = nods[v].p)
			spa(v), nods[v].c[0] = last, calc(v), last = v;
		spa(u);
		return last;
	}

	void mkR(ll u) { // make root of its tree
		exv(u), nods[u].rev ^= 1;
	}
	ll getR(ll u) { // get root of u tree
		exv(u);
		while (nods[u].c[1]) u = nods[u].c[1];
		spa(u);
		return u;
	}
	ll father(ll u) { // father of u, 0 if u is root
		exv(u), u = nods[u].c[1];
		while (nods[u].c[0]) u = nods[u].c[0];
		return u;
	}
	ll lift(ll u, ll t) {//t ancestor of x, lift(x,1) is x father
		exv(u);
		return lift_rec(u, t);
	}
	ll depth(ll u) { // distance from u to its tree root
		exv(u);
		return nods[u].s - 1;
	}
	ll lca(ll u, ll v) { // least common ancestor, 0 if not conn
		exv(u);
		return exv(v);
	}
	bool connected(ll u, ll v) { // are u and v in the same tree
		exv(u), exv(v);
		return u == v || nods[u].p != 0;
	}
	void cut(ll u) { // cuts u from father, becoming a root
		exv(father(u)), nods[u].p = 0;
	}
	// The following functions change the root
	void link(ll u, ll v) { // add edge between u and v
		mkR(u), nods[u].p = v;
	}
	void cut(ll u, ll v) { // remove edge u v
		mkR(u), cut(v);
	}
	T query(ll u, ll v) { // query on path from u to v
		mkR(u), exv(v);
		return get(v);
	}
	void upd(ll u, ll v, L d) { // modify path from u to v
		mkR(u), exv(v), nods[v].d = comb(nods[v].d, d);
	}
};
/// END content

int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll N, Q;
	cin >> N >> Q;

	vi vals(N);
	for (ll& a : vals) {
		cin >> a;
	}
	LinkCutTree lct(vals);

	fore(i, 0, N - 1) {
		ll a, b;
		cin >> a >> b;
		lct.link(a + 1, b + 1);
	}

	while (Q--) {
		ll t;
		cin >> t;
		if (t == 0) {
			ll u, v, w, x;
			cin >> u >> v >> w >> x;
			lct.cut(u + 1, v + 1);
			lct.link(w + 1, x + 1);
		} else if (t == 1) {
			ll p, x;
			cin >> p >> x;
			lct.upd(p + 1, p + 1, x);
		} else {
			ll u, v;
			cin >> u >> v;
			ll val = lct.query(u + 1, v + 1);
			cout << val << '\n';
		}
	}
}
