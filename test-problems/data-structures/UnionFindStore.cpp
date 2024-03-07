// Problem: https://cses.fi/problemset/task/1676
// Submission: https://cses.fi/problemset/result/8650362/
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

struct D {
	ll sz = 1;
};
struct UFStore {
	void merge(D& large, const D& small) { large.sz += small.sz; }
	vi e;
	vector<D> d;
	UFStore(ll n) : e(n, -1), d(n) {}
	UFStore(vector<D>& d) : e(SZ(d), -1), d(d) {}
	D& find(ll x) { return d[repr(x)]; }
	ll repr(ll x) { return e[x] < 0 ? x : e[x] = repr(e[x]); }
	bool join(ll a, ll b) {
		a = repr(a), b = repr(b);
		if (a == b) return false;
		if (e[a] > e[b]) swap(a, b);
		e[a] += e[b], e[b] = a, merge(d[a], d[b]);
		return true;
	}
};

int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll n, m;
	cin >> n >> m;
	ll largest = 1, cnt = n;
	UFStore uf(n);
	fore(i, 0, m) {
		ll a, b;
		cin >> a >> b;
		a--, b--;
		if (uf.join(a, b)) cnt--;
		largest = max(largest, uf.find(a).sz);
		cout << cnt << " " << largest << "\n";
	}
}
