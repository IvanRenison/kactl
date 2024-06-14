// Problem: https://codeforces.com/contest/161/problem/D
// Status: Accepted
// Submission: https://codeforces.com/contest/161/submission/262430160
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


ll solve(ll k, const vector<ii> edges) {
	ll n = SZ(edges) + 1;

	vector<vi> g(n);
	for (auto [a, b] : edges) {
		g[a].pb(b), g[b].pb(a);
	}

	vi ct = centroidTree(g);
	vector<vi> suns(n);
	fore(i, 0, n) {
		if (ct[i] != -1) {
			suns[ct[i]].pb(i);
		}
	}

	function<void(ll, ll, ll, map<ll, ll>&, set<ll>&)> depthCounter =
			[&](ll x, ll p, ll d, map<ll, ll>& c, set<ll>& valid) {
		c[d]++;
		for (ll y : g[x]) if (y != p && valid.count(y)) {
			depthCounter(y, x, d + 1, c, valid);
		}
	};

	ll ans = 0;

	fore(u, 0, n) {
		set<ll> subtree;
		{
			vi s = {u};
			while (!s.empty()) {
				ll x = s.back();
				s.pop_back();
				subtree.insert(x);
				for (ll y : suns[x]) {
					s.pb(y);
				}
			}
		}


		ll d = SZ(g[u]);
		vector<map<ll, ll>> cs(d);
		fore(j, 0, d) {
			ll v = g[u][j];
			if (v != ct[u] && subtree.count(v)) {
				depthCounter(g[u][j], u, 1, cs[j], subtree);
			}
		}

		map<ll, ll> c_comb;
		for (auto& c : cs) {
			for (auto [d, cnt] : c) {
				c_comb[d] += cnt;
			}
		}

		ans += c_comb[k] * 2;

		for (auto& c : cs) {
			for (auto [x, cnt] : c) {
				if (x < k) {
					ll e = k - x;
					ans += cnt * (c_comb[e] - c[e]);
				}
			}
		}
	}


	ans /= 2;

	return ans;
}

int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll n, k;
	cin >> n >> k;
	vector<ii> edges(n - 1);
	for (auto& [a, b] : edges) {
		cin >> a >> b;
		a--, b--;
	}

	ll ans = solve(k, edges);
	cout << ans << '\n';
}