// Problem: https://codeforces.com/gym/101064/problem/E
// Status: Accepted
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

/// content/strings/AhoCorasick.h
struct AhoCorasick {
/// START diff
	enum {alpha = 10, first = 'a'}; // change this!
/// END diff
	struct vertex {
		vi next = vi(alpha, -1), go = vi(alpha, -1);
		ll p, link, pch;
		vi leaf;
		vertex(ll p = -1, ll pch = -1) : p(p), link(-1), pch(pch) {}
	};
	vector<vertex> t;
	AhoCorasick() : t(1) {}
	ll add_string(string& s, ll id) {
		ll v = 0;
		for (char c : s) {
			if (t[v].next[c - first] == -1) {
				t[v].next[c - first] = SZ(t);
				t.pb(vertex(v, c - first));
			}
			v = t[v].next[c - first];
		}
		t[v].leaf.pb(id);
		return v;
	}
	ll get_link(ll v) {
		if (t[v].link < 0) {
			if (!v || !t[v].p) t[v].link = 0;
			else t[v].link = go(get_link(t[v].p), t[v].pch + first);
		}
		return t[v].link;
	}
	ll go(ll v, char c) {
		if (t[v].go[c - first] == -1) {
			if (t[v].next[c - first] != -1) t[v].go[c - first] = t[v].next[c - first];
			else t[v].go[c - first] = v == 0 ? 0 : go(get_link(v), c);
		}
		return t[v].go[c - first];
	}
};
/// END content

string solve(vector<string>& horribles, vector<string>& kinds) {
	ll N = SZ(horribles), M = SZ(kinds);

	AhoCorasick ac;

	fore(i, 0, N) ac.add_string(horribles[i], i);
	fore(i, 0, M) ac.add_string(kinds[i], i + N);

	ll n = SZ(ac.t);

	vi leafs(n, -1); // dp of leafs including the ones of links, returns bits
	function<ll(ll)> get_leafs = [&](ll u) -> ll {
		ll& ans = leafs[u];
		if (ans != -1) return ans;
		ans = 0;
		for (ll i : ac.t[u].leaf) ans |= 1 << i;
		if (u != 0) ans |= get_leafs(ac.get_link(u));
		return ans;
	};


	vector<vector<tuple<ll, ll, char>>> vis(n, vector<tuple<ll, ll, char>>(1 << N, {-1, -1, '#'}));
	queue<ii> s;
	s.push({0, 0});
	vis[0][0] = {0, 0, '@'};
	while (!s.empty()) {
		auto [u, x] = s.front();
		s.pop();

		for (char c = 'a'; c <= 'j'; c++) {
			ll v = ac.go(u, c);
			ll y = x | get_leafs(v);
			if (y < (1 << N) && get<0>(vis[v][y]) == -1) {
				vis[v][y] = {u, x, c};
				s.push({v, y});
			}

			if (y == (1 << N) - 1) { // We found a solution
				string ans;
				while (v != 0 || y != 0) {
					auto [p, x, c] = vis[v][y];
					ans += c;
					v = p;
					y = x;
				}
				reverse(ALL(ans));
				return ans;
			}
		}
	}

	return "-";
}

int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll N, M;
	cin >> N >> M;
	vector<string> horribles(N), kinds(M);
	for (string& s : horribles) cin >> s;
	for (string& s : kinds) cin >> s;
	string ans = solve(horribles, kinds);
	cout << ans << '\n';
}
/*
5 0
cabbcab
abbc
cabbcaba
abaabaac
a

*/