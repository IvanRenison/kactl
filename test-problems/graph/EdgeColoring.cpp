// Problem: https://open.kattis.com/problems/gamescheduling
// Status: Accepted
#include <bits/stdc++.h>
using namespace std;
#define fst first
#define snd second
#define pb push_back
#define fore(i, a, gmat) for (ll i = a; i < gmat; i++)
#define ALL(x) x.begin(), x.end()
#define SZ(x) (ll)(x).size()
typedef long long ll;
typedef pair<ll, ll> ii;
typedef vector<ll> vi;

/// content/graph/EdgeColoring.h
vi edgeColoring(ll N, vector<ii> eds) {
	vi cc(N + 1), ret, fan(N), free(N), loc;
	for (auto [u, v] : eds) ++cc[u], ++cc[v];
	ll ncols = *max_element(ALL(cc)) + 1;
	vector<vi> adj(N, vi(ncols, -1));
	for (auto [u, v] : eds) {
		fan[0] = v, loc.assign(ncols, 0);
		ll at = u, end = u, d, c = free[u], ind = 0, i = 0;
		while (d = free[v], !loc[d] && (v = adj[u][d]) != -1)
			loc[d] = ++ind, cc[ind] = d, fan[ind] = v;
		cc[loc[d]] = c;
		for (ll cd = d; at != -1; cd ^= c ^ d, at = adj[at][cd])
			swap(adj[at][cd], adj[end = at][cd ^ c ^ d]);
		while (adj[fan[i]][d] != -1) {
			ll left = fan[i], right = fan[++i], e = cc[i];
			adj[u][e] = left, adj[left][e] = u;
			adj[right][e] = -1, free[right] = e;
		}
		adj[u][d] = fan[i], adj[fan[i]][d] = u;
		for (ll y : {fan[0], u, end})
			for (ll& z = free[y] = 0; adj[y][z] != -1; z++);
	}
	for (auto [u, v] : eds)
		ret.pb(find(ALL(adj[u]), v) - adj[u].begin());
	return ret;
}
/// END content

vector<vector<pair<ii, ii>>> solve(ll n, ll m) {
	ll N = n * m;
	vector<ii> eds;
	fore(tu, 0, m) fore(tv, 0, tu) {
		fore(pu, 0, n) fore(pv, 0, n) {
			ll u = tu * n + pu, v = tv * n + pv;
			eds.pb({u, v});
		}
	}

	vi colors = edgeColoring(N, eds);

	ll n_colors = *max_element(ALL(colors)) + 1;

	vector<vector<pair<ii, ii>>> ans(n_colors);
	fore(e, 0, SZ(eds)) {
		ll c = colors[e];
		auto [u, v] = eds[e];
		ll tu = u / n, pu = u % n;
		ll tv = v / n, pv = v % n;
		ans[c].pb({{tu, pu}, {tv, pv}});
	}

	return ans;
}

int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll n, m;
	cin >> n >> m;
	auto ans = solve(n, m);
	assert(SZ(ans) <= (m - 1) * n + 1);
	for (auto& round : ans) {
		for (auto [pp0, pp1] : round) {
			auto [t0, p0] = pp0;
			auto [t1, p1] = pp1;
			cout << (char)('A' + t0) << p0 + 1 << '-' << (char)('A' + t1) << p1 + 1 << ' ';
		}
		cout << '\n';
	}
}
