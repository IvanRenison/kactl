// Problem: https://open.kattis.com/problems/shortestpath3
// Status: Accepted
// Test with shuffled vertices
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

/// content/graph/BellmanFord.h
const ll inf = LLONG_MAX;
struct Ed { ll a, b, w, s() { return a < b ? a : -a; }};
struct Node { ll dist = inf, prev = -1; };

void bellmanFord(vector<Node>& nodes, vector<Ed>& eds, ll s) {
	nodes[s].dist = 0;
	sort(ALL(eds), [](Ed a, Ed b) { return a.s() < b.s(); });
	/// START diff
	ll lim = SZ(nodes) / 3 + 100; // /3+100 with shuffled vertices
	/// END diff
	fore(i,0,lim) for (Ed ed : eds) {
		Node cur = nodes[ed.a], &dest = nodes[ed.b];
		if (abs(cur.dist) == inf) continue;
		ll d = cur.dist + ed.w;
		if (d < dest.dist) dest = {i < lim-1 ? d : -inf, ed.a};
	}
	fore(i,0,lim) for (Ed e : eds)
		if (nodes[e.a].dist == -inf) nodes[e.b].dist = -inf;
}
/// END content

int main() {
	cin.tie(0)->sync_with_stdio(0);
	while (true) {
		ll n, m, q, s;
		cin >> n >> m >> q >> s;
		if (n == 0) break;

		vi perm(n);
		iota(ALL(perm), 0);
		random_shuffle(ALL(perm));

		s = perm[s];

		vector<Node> nodes(n);
		vector<Ed> eds(m);

		for (auto& [u, v, w] : eds) {
			cin >> u >> v >> w;
			u = perm[u], v = perm[v];
		}

		bellmanFord(nodes, eds, s);

		while (q--) {
			ll dest;
			cin >> dest;
			dest = perm[dest];
			if (nodes[dest].dist == inf) {
				cout << "Impossible\n";
			} else if (nodes[dest].dist == -inf) {
				cout << "-Infinity\n";
			} else {
				cout << nodes[dest].dist << "\n";
			}
		}
		cout << "\n";
	}
}
