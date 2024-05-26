// Problem: https://cses.fi/problemset/task/2079
// Status: ACCEPTED
// Submission: https://cses.fi/problemset/result/9366219/
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

/// content/graph/CentroidTree.h
vi centroidTree(vector<vi>& g) {
	ll n = SZ(g);
	vector<bool> vis(n, false);
	vi fat(n), szt(n);
	function<ll(ll, ll)> calcsz = [&](ll x, ll f) {
		szt[x] = 1;
		for (ll y : g[x]) if (y != f && !vis[y])
			szt[x] += calcsz(y, x);
		return szt[x];
	};
	function<void(ll, ll, ll)> dfs = [&](ll x, ll f, ll sz) {
		if (sz < 0) sz = calcsz(x, -1);
		for (ll y : g[x]) if (!vis[y] && szt[y] * 2 >= sz) {
			szt[x] = 0;
			dfs(y, f, sz);
			return;
		}
		vis[x] = true;
		fat[x] = f;
		for (ll y : g[x]) if (!vis[y]) dfs(y, x, -1);
	};
	dfs(0, -1, -1);
	return fat;
}
/// END content


ll solve(const vector<ii> edges) {
	ll n = SZ(edges) + 1;

	vector<vi> g(n);
	for (auto [a, b] : edges) {
		g[a].pb(b), g[b].pb(a);
	}

	vi ct = centroidTree(g);

	ll total_center = -1;
	fore(i, 0, n) {
		if (ct[i] == -1) {
			total_center = i;
			break;
		}
	}

	assert(total_center != -1);

	return total_center;
}

int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll n;
	cin >> n;
	vector<ii> edges(n - 1);
	for (auto& [a, b] : edges) {
		cin >> a >> b;
		a--, b--;
	}

	ll ans = solve(edges);
	cout << ans + 1 << '\n';
}