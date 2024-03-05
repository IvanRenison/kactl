// Problem: https://cses.fi/problemset/task/1675/
// Submission: https://cses.fi/problemset/result/8586074/
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

struct UF {
    vi e;
    UF(ll n) : e(n, -1) {}
    ll find(ll x) { return e[x] < 0 ? x : e[x] = find(e[x]); }
    bool join(ll a, ll b) {
        a = find(a), b = find(b);
        if (a == b) return false;
        if (e[a] > e[b]) swap(a, b);
        e[a] += e[b], e[b] = a;
        return true;
    }
};
auto kruskal(vector<pair<ll, ii>>& es, ll n) {
    sort(ALL(es));
    UF uf(n);
    ll r = 0;
    for (auto [w, p] : es) {
        auto [u, v] = p;
        if (uf.join(u, v)) {
            r += w;
            // {w, {u, v}} is in MST
        }
    }
    return make_pair(r, uf);
}

int main() {
    cin.tie(0)->sync_with_stdio(0);

    ll n, m;
    cin >> n >> m;
    vector<pair<ll, ii>> es(m);
    fore(i, 0, m) {
        ll a, b, c;
        cin >> a >> b >> c;
        a--, b--;
        es[i] = {c, {a, b}};
    }

    auto [r, uf] = kruskal(es, n);
    fore(i, 0, n) if (uf.find(i) != uf.find(0)) {
        cout << "IMPOSSIBLE\n";
        return 0;
    }
    cout << r << "\n";
}
