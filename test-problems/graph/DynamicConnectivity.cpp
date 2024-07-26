// Problem: https://codeforces.com/contest/1140/problem/F
// Status: Accepted
// Submission: https://codeforces.com/contest/1140/submission/272721785
#include <bits/stdc++.h>
using namespace std;
#define fst first
#define snd second
#define pb push_back
#define fore(i, a, gmat) for (ll i = a; i < gmat; i++)
#define ALL(x) begin(x), end(x)
#define SZ(x) (ll)(x).size()
typedef long long ll;
typedef pair<ll, ll> ii;
typedef vector<ll> vi;

/// content/data-structures/UnionFindRollbackStore.h
typedef ll T; // Global data
/// START diff
typedef ii D; // Component data
/// END diff
struct RSUF {
	T ans; // Global data initial value, set in constructor
	/// START diff
	void merge(D& large, const D& small) {
		ans -= large.fst * large.snd + small.fst * small.snd;
		large.fst += small.fst;
		large.snd += small.snd;
		ans += large.fst * large.snd;
	}
	/// END diff

	ll n;
	vi e; vector<D> d;
	vector<tuple<ll,ll,ll,D,T>> st;
	/// START diff
	RSUF(ll n) : ans(0), n(n), e(n, -1), d(n) {}
	RSUF(vector<D>& d) : ans(0), n(SZ(d)), e(n,-1), d(d){}
	/// END diff
	ll size(ll x) { return -e[find(x)]; }
	ll find(ll x) { return e[x] < 0 ? x : find(e[x]); }
	ll time() { return SZ(st); }
	D get(ll x) { return d[find(x)]; }
	void rollback(ll t) {
		while (SZ(st) > t) {
			auto [a, b, s, v, t] = st.back();
			st.pop_back();
			d[a] = v, e[a] -= e[b] = s, ans = t;
		}
	}
	bool join(ll a, ll b) {
		a = find(a), b = find(b);
		if (a == b) return false;
		if (e[a] > e[b]) swap(a, b);
		st.pb({a, b, e[b], d[a], ans});
		merge(d[a], d[b]);
		e[a] += e[b], e[b] = a;
		return true;
	}
};
/// END content

/// content/graph/DynamicConnectivity.h
enum { ADD, DEL, QUERY };
struct Query { ll type, x, y; }; // You can add stuff for QUERY
struct DynCon {
	vector<Query> q;
	RSUF uf;
	vi mt;
	map<ii, ll> last;
	vector<T> ans;
	DynCon(ll n) : uf(n) {}
	DynCon(vector<D>& d) : uf(d) {}
	void add(ll x, ll y) {
		if (x > y) swap(x, y);
		mt.pb(-1);
		last[{x, y}] = SZ(q);
		q.pb({ADD, x, y});
	}
	void remove(ll x, ll y) {
		if (x > y) swap(x, y);
		ll pr = last[{x, y}];
		mt[pr] = SZ(q);
		mt.pb(pr);
		q.pb({DEL, x, y});
	}
	void query() { // Add parameters if needed
		q.pb({QUERY, -1, -1});
		mt.pb(-1);
	}
	void process() { // Answers all queries in order
		if (q.empty()) return;
		fore(i, 0, SZ(q))
			if (q[i].type == ADD && mt[i] < 0) mt[i] = SZ(q);
		go(0, SZ(q));
	}
	void go(ll s, ll e) {
		if (s + 1 == e) {
			if (q[s].type == QUERY) { // Answer query using DSU
				ans.pb(uf.ans); // Maybe you want to use uf.get(x)
			}                 // for some x stored in Query
			return;
		}
		ll k = uf.time(), m = (s + e) / 2;
		for (ll i = e; --i >= m;)
			if (0 <= mt[i] && mt[i] < s) uf.join(q[i].x, q[i].y);
		go(s, m);
		uf.rollback(k);
		for (ll i = m; --i >= s;)
			if (mt[i] >= e) uf.join(q[i].x, q[i].y);
		go(m, e);
		uf.rollback(k);
	}
};
/// END content

vi solve(const vector<ii>& queries) {
	ll q = queries.size();

	map<ll, ll> xmap, ymap;
	{
		vi xs(q), ys(q);
		fore(i, 0, q) {
			auto [x, y] = queries[i];
			xs[i] = x, ys[i] = y;
		}

		sort(ALL(xs));
		sort(ALL(ys));

		fore(i, 0, q) {
			xmap[xs[i]] = i;
			ymap[ys[i]] = q + i;
		}
	}

	set<ii> ps;

	vector<ii> vals(2 * q);
	fore(i, 0, q) {
		vals[i] = {1, 0};
		vals[q + i] = {0, 1};
	}

	DynCon dc(vals);

	for (auto [x, y] : queries) {
		if (ps.count({x, y})) {
			ps.erase({x, y});
			dc.remove(xmap[x], ymap[y]);
		} else {
			ps.insert({x, y});
			dc.add(xmap[x], ymap[y]);
		}
		dc.query();
	}

	dc.process();

	return dc.ans;
}

int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll q;
	cin >> q;
	vector<ii> queries(q);
	for (auto& [x, y] : queries) {
		cin >> x >> y;
	}

	vi ans = solve(queries);
	for (ll x : ans) {
		cout << x << ' ';
	}
	cout << '\n';

}
