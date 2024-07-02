// Problem: https://judge.yosupo.jp/problem/dynamic_tree_vertex_set_path_composite
// Status: AC
// Submission: https://judge.yosupo.jp/submission/214998
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

const ll mod = 998244353;

/// content/graph/LinkCutTree.h
/// START diff
struct T {
	ll a, b;
	ll operator()(ll x) {
		return (a * x + b) % mod;
	}
};
const T neut = {1, 0};
T f(T a, T b) {
	return {(a.a * b.a) % mod, (a.a * b.b + a.b) % mod};
} // associative
//T neg(T a, T b) {  } // inverse of f for SUBTREE
/// END diff
struct SplayTree {
	struct Node {
		array<ll, 2> c = {0, 0};
		ll p = 0;
		array<T, 2> path = {neut, neut};
		T self = neut, sub = neut, vir = neut;
		bool reverse = false;
	};
	vector<Node> nods;

	SplayTree(ll n) : nods(n + 1) {}

	ll getDir(ll x) {
		ll p = nods[x].p;
		if (!p) return -1;
		if (nods[p].c[0] == x) return 0;
		return nods[p].c[1] == x ? 1 : -1;
	}
	void pushDown(ll x) {
		if (!x) return;
		if (nods[x].reverse) {
			auto [l, r] = nods[x].c;
			nods[l].reverse ^= 1, nods[r].reverse ^= 1;
			swap(nods[x].c[0], nods[x].c[1]);
			swap(nods[x].path[0], nods[x].path[1]);
			nods[x].reverse = false;
		}
	}
	void pushUp(ll x) {
		auto [l, r] = nods[x].c;
		pushDown(l), pushDown(r);
		nods[x].path[0] =
			f(f(nods[l].path[0], nods[x].self), nods[r].path[0]);
		nods[x].path[1] =
			f(f(nods[r].path[1], nods[x].self), nods[l].path[1]);
		nods[x].sub = f(
			f(f(nods[x].vir, nods[l].sub), nods[r].sub),
			nods[x].self);
	}
	void setChild(ll x, ll y, ll dir) {
		nods[x].c[dir] = y;
		nods[y].p = x;
		pushUp(x);
	}
	void rotate(ll x) {
		ll y = nods[x].p, dx = getDir(x);
		ll z = nods[y].p, dy = getDir(y);
		setChild(y, nods[x].c[!dx], dx), setChild(x, y, !dx);
		if (~dy) setChild(z, x, dy);
		nods[x].p = z;
	}
	void spa(ll x) { // splay
		for (pushDown(x); ~getDir(x); ) {
			ll y = nods[x].p, z = nods[y].p;
			pushDown(z), pushDown(y), pushDown(x);
			ll dx = getDir(x), dy = getDir(y);
			if (~dy) rotate(dx != dy ? x : y);
			rotate(x);
		}
	}
};

struct LinkCutTree : SplayTree {
	LinkCutTree(ll n) : SplayTree(n) {}
	ll access(ll x) {
		ll u = x, v = 0;
		for (; u; v = u, u = nods[u].p) {
			spa(u);
			ll& ov = nods[u].c[1];
			nods[u].vir = f(nods[u].vir, nods[ov].sub);
			/// START diff
			//nods[u].vir = neg(nods[u].vir, nods[v].sub); // SUBTREE
			/// END diff
			ov = v; pushUp(u);
		}
		return spa(x), v;
	}

	void mkR(ll u) { // make root of its tree
		access(u);
		nods[u].reverse ^= 1;
		pushDown(u);
	}
	ll lca(ll u, ll v) { // least common ancestor, 0 if not conn
		if (u == v) return u;
		access(u);
		ll ret = access(v);
		return nods[u].p ? ret : 0;
	}
	bool connected(ll u, ll v) {  // are u and v in the same tree
		return lca(u, v) > 0;
	}
	T query(ll u) { // query single element
		return nods[u].self;
	}
	/// START diff
	// T querySub(ll u) { // query SUBTREE of u
	// 	access(u);
	// 	return f(nods[u].vir, nods[u].self);
	// }
	/// END diff
	void upd(ll u, T val) { // update value of u
		access(u);
		nods[u].self = val;
		pushUp(u);
	}
	// The following functions change the root
	void link(ll u, ll v) { // add edge between u and v
		mkR(u), access(v);
		nods[v].vir = f(nods[v].vir, nods[u].sub);
		nods[u].p = v;
		pushUp(v);
	}
	void cut(ll u, ll v) { // remove edge u v
		mkR(u), access(v);
		nods[v].c[0] = nods[u].p = 0;
		pushUp(v);
	}
	T queryPath(ll u, ll v) { // query on path from u to v
		mkR(u), access(v);
		return nods[v].path[1];
	}
};
/// END content


int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll N, Q;
	cin >> N >> Q;

	LinkCutTree lct(N);

	fore(i, 0, N) {
		ll a, b;
		cin >> a >> b;
		lct.upd(i + 1, {a, b});
	}

	fore(_, 0, N - 1) {
		ll u, v;
		cin >> u >> v;
		lct.link(u + 1, v + 1);
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
			ll p, c, d;
			cin >> p >> c >> d;
			lct.upd(p + 1, {c, d});
		} else {
			ll u, v, x;
			cin >> u >> v >> x;
			ll val = lct.queryPath(u + 1, v + 1)(x);
			cout << val << '\n';
		}
	}
}
