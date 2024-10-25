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
	static const ll alpha = 10; // Size of the alphabet
/// END diff
	struct Node {
		array<ll, alpha> next, go; vi leaf; ll p, link, pch;
		Node(ll p = -1, ll pch = -1) : p(p), link(-1), pch(pch) {
			next.fill(-1), go = next;
		}
	};
	vector<Node> t;
	AhoCorasick(vector<vi>& words) : t(1) {
		fore(i, 0, SZ(words)) {
			ll v = 0;
			for (ll c : words[i]) {
				if (t[v].next[c]<0) t[v].next[c]=SZ(t),t.pb(Node(v,c));
				v = t[v].next[c];
			}
			t[v].leaf.pb(i);
		}
	}
	ll getLink(ll v) {
		if (t[v].link < 0) t[v].link = v && t[v].p ?
			go(getLink(t[v].p), t[v].pch) : 0;
		return t[v].link;
	}
	ll go(ll v, ll c) {
		if (t[v].go[c] < 0) t[v].go[c] = t[v].next[c] >= 0 ?
			t[v].next[c] : v ? go(getLink(v),c) : 0;
		return t[v].go[c];
	}
};
/// END content

string solve(vector<string>& horribles, vector<string>& kinds) {
	ll N = SZ(horribles), M = SZ(kinds);

	vector<vi> words(N + M);
	fore(i, 0, N) {
		for (char c : horribles[i]) words[i].pb(c - 'a');
	}
	fore(i, 0, M) {
		for (char c : kinds[i]) words[N + i].pb(c - 'a');
	}

	AhoCorasick ac(words);

	ll n = SZ(ac.t);

	vi leafs(n, -1); // dp of leafs including the ones of links, returns bits
	auto get_leafs = [&](ll u, auto&& get_leafs) -> ll {
		ll& ans = leafs[u];
		if (ans != -1) return ans;
		ans = 0;
		for (ll i : ac.t[u].leaf) ans |= 1 << i;
		if (u != 0) ans |= get_leafs(ac.getLink(u), get_leafs);
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
			ll v = ac.go(u, c - 'a');
			ll y = x | get_leafs(v, get_leafs);
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
