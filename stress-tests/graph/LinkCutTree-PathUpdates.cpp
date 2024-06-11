#include "../utilities/template.h"

#include "../../content/graph/LinkCutTree-PathUpdates.h"

namespace slow {
	typedef ll T; // T: data type, L: lazy type
	typedef ll L;
	const L lneut = 0;
	const L tneut = 0;
	T f(T a, T b) { return T{a + b}; } // operation
	// new st according to lazy
	T apply(T v, L l, ll len) { return T{v + l * len}; }
	// cumulative effect of lazy
	L comb(L a, L b) { return L{a + b}; }

	struct Slow {
		ll n;
		vector<T> vals;
		set<ii> edges;

		Slow(ll n) : n(n), vals(n, tneut) {}
		Slow(vector<T>& vals) : n(SZ(vals)), vals(vals) {}

		bool connected(ll u, ll v) {
			vector<vi> adj(n);
			for (auto [u, v] : edges) {
				adj[u].pb(v), adj[v].pb(u);
			}

			vector<ii> s = {{u, u}};

			while (!s.empty()) {
				auto [x, p] = s.back();
				s.pop_back();

				if (x == v) return true;

				for (ll y : adj[x]) {
					if (y != p) s.pb({y, x});
				}
			}

			return false;
		}
		void link(ll u, ll v) {
			assert(!connected(u, v));
			edges.insert(minmax(u, v));
		}
		void cut(ll u, ll v) {
			assert(edges.count(minmax(u, v)));
			edges.erase(minmax(u, v));
		}
		T query(ll u, ll v) {
			if (u == v) return vals[u];

			vector<vi> adj(n);
			for (auto [u, v] : edges) {
				adj[u].pb(v), adj[v].pb(u);
			}

			vector<ll> ps(n, -1);
			ps[u] = -2;
			vi s = {u};
			while (!s.empty()) {
				ll x = s.back();
				s.pop_back();

				if (x == v) {
					break;
				}

				for (ll y : adj[x]) {
					if (ps[y] == -1) {
						ps[y] = x;
						s.pb(y);
					}
				}
			}

			assert(ps[v] != -1);

			T ans = tneut;

			for (ll x = v; x != -2; x = ps[x]) {
				ans = f(ans, vals[x]);
			}

			return ans;
		}
		void upd(ll u, ll v, L d) {
			if (u == v) {
				vals[u] = apply(vals[u], d, 1);
				return;
			}

			vector<vi> adj(n);
			for (auto [u, v] : edges) {
				adj[u].pb(v), adj[v].pb(u);
			}

			vector<ll> ps(n, -1);
			ps[u] = -2;
			vi s = {u};
			while (!s.empty()) {
				ll x = s.back();
				s.pop_back();

				if (x == v) break;

				for (ll y : adj[x]) {
					if (ps[y] == -1) {
						ps[y] = x;
						s.pb(y);
					}
				}
			}

			assert(ps[v] != -1);

			for (ll x = v; x != -2; x = ps[x]) {
				vals[x] = apply(vals[x], d, 1);
			}
		}
	};
};

void testCase() {
	ll n = rand() % 100 + 1;

	vector<ll> vals(n);
	fore(i, 0, n) vals[i] = rand() % 100;

	slow::Slow slow(vals);
	LinkCutTree lct(vals);

	set<ii> edges;

	fore(_, 0, 1000) {
		ll t = rand() % 4;
		if (t == 0) { // link
			ll u = rand() % n, v = rand() % n;
			bool conn = lct.connected(u + 1, v + 1);
			bool conn2 = slow.connected(u, v);
			assert(conn == conn2);
			if (conn) continue;
			lct.link(u + 1, v + 1);
			slow.link(u, v);
			edges.insert(minmax(u, v));
		} else if (t == 1) { // cut
			if (edges.empty()) continue;
			// get random element from edges
			auto [u, v] = *next(edges.begin(), rand() % SZ(edges));
			assert(lct.connected(u + 1, v + 1));
			lct.cut(u + 1, v + 1);
			slow.cut(u, v);
			edges.erase(minmax(u, v));
		} else if (t == 2) { // query
			ll u = rand() % n, v = rand() % n;
			bool conn = lct.connected(u + 1, v + 1);
			bool conn2 = slow.connected(u, v);
			assert(conn == conn2);
			if (!conn) continue;
			ll ans = lct.query(u + 1, v + 1);
			ll ans2 = slow.query(u, v);
			assert(ans == ans2);
		} else { // modify
			ll u = rand() % n, v = rand() % n;
			bool conn = lct.connected(u + 1, v + 1);
			bool conn2 = slow.connected(u, v);
			assert(conn == conn2);
			if (!conn) continue;
			ll d = rand() % 100;
			lct.upd(u + 1, v + 1, d);
			slow.upd(u, v, d);
		}
	}
}

int main() {
	fore(_, 0, 1000) testCase();
	cout << "Tests passed!" << endl;
}
