#include "../utilities/template.h"

#include "../../content/graph/LinkCutTree.h"

namespace slow {
	typedef ll T;
	const T neut = 0;
	T f(T a, T b) { return a + b; } // associative & commutative
	// Inverse is not needed because we will traverse the tree en each query

	struct Slow { // indexed from 0
		ll n;
		vector<T> vals;
		vector<ii> edges;
		vi root; // root ef the tree of each node

		Slow(ll n) : n(n), vals(n, neut), root(n) {
			iota(ALL(root), 0);
		}

		void mkR(ll u) {
			vector<vi> adj(n);
			for (auto [v, u] : edges) adj[v].pb(u), adj[u].pb(v);

			vector<bool> vis(n);
			vis[u] = true;
			vi s = {u};
			while (!s.empty()) {
				ll v = s.back();
				s.pop_back();
				root[v] = u;
				for (ll w : adj[v]) if (!vis[w]) {
					s.pb(w);
					vis[w] = true;
				}
			}
		}

		ll lca(ll u, ll v) {
			ll r = root[u];
			if (root[v] != r) return -1;

			vector<vi> adj(n);
			for (auto [v, u] : edges) adj[v].pb(u), adj[u].pb(v);

			vi parents(n, -1), depths(n, -1);
			depths[r] = 0;

			vi s = {r};
			while (!s.empty()) {
				ll w = s.back();
				s.pop_back();
				for (ll x : adj[w]) if (depths[x] == -1) {
					depths[x] = depths[w] + 1;
					parents[x] = w;
					s.pb(x);
				}
			}

			if (depths[u] < depths[v]) swap(u, v);
			while (depths[u] > depths[v]) u = parents[u];
			while (u != v) u = parents[u], v = parents[v];
			return u;
		}

		bool connected(ll u, ll v) {
			return root[u] == root[v];
		}

		T query(ll u) {
			return vals[u];
		}

		T querySub(ll u) {
			ll r = root[u];

			vector<vi> adj(n);
			for (auto [v, u] : edges) adj[v].pb(u), adj[u].pb(v);

			vi parents(n, -1);
			parents[r] = r;

			T ans = neut;

			// Pairs are (node, is in subtree of u)
			vector<pair<ll, bool>> s = {{r, r == u}};
			while (!s.empty()) {
				auto [v, b] = s.back();
				s.pop_back();
				if (b) ans = f(ans, vals[v]);
				for (ll w : adj[v]) if (parents[w] == -1) {
					parents[w] = v;
					s.pb({w, b || w == u});
				}
			}

			return ans;
		}

		void upd(ll u, T val) {
			vals[u] = val;
		}

		void link(ll u, ll v) {
			assert(!connected(u, v));
			edges.pb({u, v});
			ll r = root[u];
			fore(w, 0, n) {
				if (root[w] == r) root[w] = root[v];
			}
		}

		void cut(ll u, ll v) {
			assert(connected(u, v));
			edges.erase(find(ALL(edges), ii{u, v}));

			vector<vi> adj(n);
			for (auto [v, u] : edges) adj[v].pb(u), adj[u].pb(v);

			vector<bool> vis(n);
			vis[u] = true;
			vi s = {u};
			while (!s.empty()) {
				ll w = s.back();
				s.pop_back();
				root[w] = u;
				for (ll x : adj[w]) if (!vis[x]) {
					s.pb(x);
					vis[x] = true;
				}
			}

			vis[v] = true;
			s = {v};
			while (!s.empty()) {
				ll w = s.back();
				s.pop_back();
				root[w] = v;
				for (ll x : adj[w]) if (!vis[x]) {
					s.pb(x);
					vis[x] = true;
				}
			}
		}

		T queryPath(ll u, ll v) {
			if (u == v) return vals[u];
			vector<vi> adj(n);
			for (auto [u, v] : edges) {
				adj[u].pb(v), adj[v].pb(u);
			}

			vector<ll> parents(n, -1);
			parents[u] = -2;
			vi s = {u};
			while (!s.empty()) {
				ll x = s.back();
				s.pop_back();

				if (x == v) break;

				for (ll y : adj[x]) {
					if (parents[y] == -1) {
						parents[y] = x;
						s.pb(y);
					}
				}
			}
			assert(parents[v] != -1);

			T ans = neut;
			for (ll x = v; x != -2; x = parents[x]) {
				ans = f(ans, vals[x]);
			}
			return ans;
		}
	};
};

void testCase() {
	ll n = rand() % 100 + 1;

	slow::Slow slow(n);
	LinkCutTree lct(n);

	fore(i, 0, n) {
		ll a = rand() % 100;
		lct.upd(i + 1, a);
		slow.upd(i, a);
	}

	set<ii> edges;

	fore(_, 0, 1000) {
		ll t = rand() % 6;

		if (t == 0) { // lca
			ll u = rand() % n, v = rand() % n;
			bool conn = lct.connected(u + 1, v + 1);
			bool conn2 = slow.connected(u, v);
			assert(conn == conn2);
			if (conn) {
				ll lca = lct.lca(u + 1, v + 1) - 1;
				ll lca2 = slow.lca(u, v);
				assert(lca == lca2);
			}
		} else if (t == 1) { // query point
			ll u = rand() % n;
			ll ans = lct.query(u + 1);
			ll ans2 = slow.query(u);
			assert(ans == ans2);
		} else if (t == 2) { // query subtree
			ll u = rand() % n;
			ll ans = lct.querySub(u + 1);
			ll ans2 = slow.querySub(u);
			assert(ans == ans2);
		} else if (t == 3) { // update
			ll u = rand() % n, a = rand() % 100;
			lct.upd(u + 1, a);
			slow.upd(u, a);
		} else if (t == 4) { // link
			ll u = rand() % n, v = rand() % n;
			bool conn = lct.connected(u + 1, v + 1);
			bool conn2 = slow.connected(u, v);
			assert(conn == conn2);
			if (!conn) {
				lct.link(u + 1, v + 1);
				slow.link(u, v);
				edges.insert({u, v});
				// Set new root
				lct.mkR(u + 1);
				slow.mkR(u);
			}
		} else if (t == 5) { // cut
			if (edges.empty()) continue;
			auto [u, v] = *next(edges.begin(), rand() % SZ(edges));
			assert(lct.connected(u + 1, v + 1));
			lct.cut(u + 1, v + 1);
			slow.cut(u, v);
			edges.erase({u, v});
			// Set new root
			lct.mkR(u + 1);
			slow.mkR(u);
		} else { // query path
			ll u = rand() % n, v = rand() % n;
			bool conn = lct.connected(u + 1, v + 1);
			bool conn2 = slow.connected(u, v);
			assert(conn == conn2);
			if (conn) {
				ll ans = lct.queryPath(u + 1, v + 1);
				ll ans2 = slow.queryPath(u, v);
				assert(ans == ans2);
				// Set new root
				lct.mkR(u + 1);
				slow.mkR(u);
			}
		}
	}
}

namespace nonCommutative { // test a non commutative operation
	typedef string T;
	const T neut = "";
	T f(T a, T b) { return a + b; } // associative
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
				// nods[u].vir = neg(nods[u].vir, nods[v].sub); // SUBTREE
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
		/* T querySub(ll u) { // query SUBTREE of u
			access(u);
			return f(nods[u].vir, nods[u].self);
		} */
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

	struct Slow { // indexed from 0
		ll n;
		vector<T> vals;
		vector<ii> edges;
		vi root; // root ef the tree of each node

		Slow(ll n) : n(n), vals(n, neut), root(n) {
			iota(ALL(root), 0);
		}

		void mkR(ll u) {
			vector<vi> adj(n);
			for (auto [v, u] : edges) adj[v].pb(u), adj[u].pb(v);

			vector<bool> vis(n);
			vis[u] = true;
			vi s = {u};
			while (!s.empty()) {
				ll v = s.back();
				s.pop_back();
				root[v] = u;
				for (ll w : adj[v]) if (!vis[w]) {
					s.pb(w);
					vis[w] = true;
				}
			}
		}

		ll lca(ll u, ll v) {
			ll r = root[u];
			if (root[v] != r) return -1;

			vector<vi> adj(n);
			for (auto [v, u] : edges) adj[v].pb(u), adj[u].pb(v);

			vi parents(n, -1), depths(n, -1);
			depths[r] = 0;

			vi s = {r};
			while (!s.empty()) {
				ll w = s.back();
				s.pop_back();
				for (ll x : adj[w]) if (depths[x] == -1) {
					depths[x] = depths[w] + 1;
					parents[x] = w;
					s.pb(x);
				}
			}

			if (depths[u] < depths[v]) swap(u, v);
			while (depths[u] > depths[v]) u = parents[u];
			while (u != v) u = parents[u], v = parents[v];
			return u;
		}

		bool connected(ll u, ll v) {
			return root[u] == root[v];
		}

		T query(ll u) {
			return vals[u];
		}

		T querySub(ll u) {
			ll r = root[u];

			vector<vi> adj(n);
			for (auto [v, u] : edges) adj[v].pb(u), adj[u].pb(v);

			vi parents(n, -1);
			parents[r] = r;

			T ans = neut;

			// Pairs are (node, is in subtree of u)
			vector<pair<ll, bool>> s = {{r, r == u}};
			while (!s.empty()) {
				auto [v, b] = s.back();
				s.pop_back();
				if (b) ans = f(ans, vals[v]);
				for (ll w : adj[v]) if (parents[w] == -1) {
					parents[w] = v;
					s.pb({w, b || w == u});
				}
			}

			return ans;
		}

		void upd(ll u, T val) {
			vals[u] = val;
		}

		void link(ll u, ll v) {
			assert(!connected(u, v));
			edges.pb({u, v});
			ll r = root[u];
			fore(w, 0, n) {
				if (root[w] == r) root[w] = root[v];
			}
		}

		void cut(ll u, ll v) {
			assert(connected(u, v));
			edges.erase(find(ALL(edges), ii{u, v}));

			vector<vi> adj(n);
			for (auto [v, u] : edges) adj[v].pb(u), adj[u].pb(v);

			vector<bool> vis(n);
			vis[u] = true;
			vi s = {u};
			while (!s.empty()) {
				ll w = s.back();
				s.pop_back();
				root[w] = u;
				for (ll x : adj[w]) if (!vis[x]) {
					s.pb(x);
					vis[x] = true;
				}
			}

			vis[v] = true;
			s = {v};
			while (!s.empty()) {
				ll w = s.back();
				s.pop_back();
				root[w] = v;
				for (ll x : adj[w]) if (!vis[x]) {
					s.pb(x);
					vis[x] = true;
				}
			}
		}

		T queryPath(ll u, ll v) {
			if (u == v) return vals[u];
			vector<vi> adj(n);
			for (auto [u, v] : edges) {
				adj[u].pb(v), adj[v].pb(u);
			}

			vector<ll> parents(n, -1);
			parents[u] = -2;
			vi s = {u};
			while (!s.empty()) {
				ll x = s.back();
				s.pop_back();

				if (x == v) break;

				for (ll y : adj[x]) {
					if (parents[y] == -1) {
						parents[y] = x;
						s.pb(y);
					}
				}
			}
			assert(parents[v] != -1);

			T ans = neut;
			for (ll x = v; x != -2; x = parents[x]) {
				ans = f(ans, vals[x]);
			}
			return ans;
		}
	};

	void testCase() {
		ll n = rand() % 50 + 1;

		Slow slow(n);
		LinkCutTree lct(n);

		fore(i, 0, n) {
			string a;
			ll k = rand() % 5;
			fore(_, 0, k) a += 'a' + rand() % 26;
			lct.upd(i + 1, a);
			slow.upd(i, a);
		}

		set<ii> edges;

		fore(_, 0, 100) {
			ll t = rand() % 5;

			if (t == 0) { // lca
				ll u = rand() % n, v = rand() % n;
				bool conn = lct.connected(u + 1, v + 1);
				bool conn2 = slow.connected(u, v);
				assert(conn == conn2);
				if (conn) {
					ll lca = lct.lca(u + 1, v + 1) - 1;
					ll lca2 = slow.lca(u, v);
					assert(lca == lca2);
				}
			} else if (t == 1) { // query point
				ll u = rand() % n;
				string ans = lct.query(u + 1);
				string ans2 = slow.query(u);
				assert(ans == ans2);
			} else if (t == 2) { // update
				ll u = rand() % n;
				string a;
				ll k = rand() % 5;
				fore(_, 0, k) a += 'a' + rand() % 26;
				lct.upd(u + 1, a);
				slow.upd(u, a);
			} else if (t == 3) { // link
				ll u = rand() % n, v = rand() % n;
				bool conn = lct.connected(u + 1, v + 1);
				bool conn2 = slow.connected(u, v);
				assert(conn == conn2);
				if (!conn) {
					lct.link(u + 1, v + 1);
					slow.link(u, v);
					edges.insert({u, v});
					// Set new root
					lct.mkR(u + 1);
					slow.mkR(u);
				}
			} else if (t == 4) { // cut
				if (edges.empty()) continue;
				auto [u, v] = *next(edges.begin(), rand() % SZ(edges));
				assert(lct.connected(u + 1, v + 1));
				lct.cut(u + 1, v + 1);
				slow.cut(u, v);
				edges.erase({u, v});
				// Set new root
				lct.mkR(u + 1);
				slow.mkR(u);
			} else { // query path
				ll u = rand() % n, v = rand() % n;
				bool conn = lct.connected(u + 1, v + 1);
				bool conn2 = slow.connected(u, v);
				assert(conn == conn2);
				if (conn) {
					string ans = lct.queryPath(u + 1, v + 1);
					string ans2 = slow.queryPath(u, v);
					assert(ans == ans2);
					// Set new root
					lct.mkR(u + 1);
					slow.mkR(u);
				}
			}
		}

	}

}

int main() {
	fore(_, 0, 1000) testCase();
	fore(_, 0, 100) nonCommutative::testCase();
	cout << "Tests passed!" << endl;
}
