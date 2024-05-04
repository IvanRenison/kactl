// Problem: https://codeforces.com/contest/893/problem/F
// Status: Accepted
// Submission: https://codeforces.com/contest/893/submission/259527347
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

/// content/data-structures/PersistentSegmentTree.h
struct Tree {
	typedef ll T;
	/// START diff
	static constexpr T neut = LONG_LONG_MAX;
	T f(T a, T b) { return min(a, b); } // (any associative fn)
	/// END diff

	vector<T> st;
	vector<ll> L, R;
	ll n, rt;
	Tree(ll n) : st(1, neut), L(1), R(1), n(n), rt(0) {}
	ll new_node(T v, ll l, ll r) {
		st.pb(v), L.pb(l), R.pb(r);
		return SZ(st) - 1;
	}
	// not necessary in most cases
	ll init(ll s, ll e, vector<T>& a) {
		if (s + 1 == e) return new_node(a[s], 0, 0);
		ll m = (s + e) / 2, l = init(s, m, a), r = init(m, e, a);
		return new_node(f(st[l], st[r]), l, r);
	}
	ll upd(ll k, ll s, ll e, ll p, T v) {
		ll ks = new_node(st[k], L[k], R[k]);
		if (s + 1 == e) {
			st[ks] = v;
			return ks;
		}
		ll m = (s + e) / 2;
		if (p < m) L[ks] = upd(L[ks], s, m, p, v);
		else R[ks] = upd(R[ks], m, e, p, v);
		st[ks] = f(st[L[ks]], st[R[ks]]);
		return ks;
	}
	T query(ll k, ll s, ll e, ll a, ll b) {
		if (e <= a || b <= s) return neut;
		if (a <= s && e <= b) return st[k];
		ll m = (s + e) / 2;
		return f(query(L[k], s, m, a, b), query(R[k], m, e, a, b));
	}
	ll init(vector<T>& a) { return init(0, n, a); }
	ll upd(ll ver, ll p, T v) {return rt = upd(ver, 0, n, p, v);}
	// upd on last root
	ll upd(ll p, T v) { return upd(rt, p, v); }
	T query(ll ver, ll a, ll b) {return query(ver, 0, n, a, b);}
};

/// END content

vector<ii> dfsOrder(ll r, const vector<vi>& adj) {
	ll n = SZ(adj);

	vector<ii> ord(n);

	ll i = 0;
	function<void(ll, ll)> dfs = [&](ll u, ll p) {
		ord[u].first = i;
		i++;

		for (ll v : adj[u]) {
			if (v != p) {
				dfs(v, u);
			}
		}

		ord[u].second = i;
	};

	dfs(r, -1);

	return ord;
}

vi solve(ll r, const vector<ii>& edges, const vi& as, const vector<ii>& queries) {
	ll n = SZ(as), q = SZ(queries);

	vector<vi> adj(n);
	for (const auto& [u, v] : edges) {
		adj[u].push_back(v), adj[v].push_back(u);
	}

	vector<vi> levels = {{r}};

	vi dep(n, -1);

	dep[r] = 0;

	while (!levels.back().empty()) {
		ll d = SZ(levels);
		levels.push_back({});

		for (ll u : levels[d - 1]) {
			for (ll v : adj[u]) {
				if (dep[v] == -1) {
					dep[v] = d;
					levels.back().push_back(v);
				}
			}
		}
	}

	levels.pop_back();

	vector<ii> ord = dfsOrder(r, adj);

	Tree st(n);

	vi versions(SZ(levels));

	fore(l, 0, SZ(levels)) {
		ll v = -1;
		for (ll u : levels[l]) {
			v = st.upd(ord[u].first, as[u]);
		}
		assert(v != -1);

		versions[l] = v;
	}

	ll last = 0;
	vi ans(q);
	fore(i, 0, q) {
		auto [p, q] = queries[i];
		ll x = (p + last) % n;
		ll k = (q + last) % n;

		ll v = versions[min(SZ(levels) - 1, dep[x] + k)];

		last = st.query(v, ord[x].first, ord[x].second);

		ans[i] = last;
	}

	return ans;
}

int main(void) {
	cin.tie(0)->sync_with_stdio(0);

	ll n, r;
	cin >> n >> r;
	r--;
	vi as(n);
	for (ll& a : as) {
		cin >> a;
	}
	vector<ii> edges(n - 1);
	for (auto& [u, v] : edges) {
		cin >> u >> v;
		--u, --v;
	}

	ll m;
	cin >> m;
	vector<ii> queries(m);
	for (auto& [p, q] : queries) {
		cin >> p >> q;
	}

	vi ans = solve(r, edges, as, queries);
	for (ll a : ans) {
		cout << a << '\n';
	}

	return EXIT_SUCCESS;
}
