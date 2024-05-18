/**
 * Author: Iván Renison
 * Date: 2024-05-18
 * License: CC0
 * Source: Folklore
 * Description: Data structure for computing queries on paths in a tree.
 * Time: construction $O(N \log N)$, queries $O(\log N)$
 * Status: untestd
 */
#pragma once

struct PathQueries { // O(n log n) preprocessing, O(log n) query
  typedef ll T;
  constexpr static T NEUT = 0;
  T op(const T& a, const T& b) {
    return max(a, b);
  } // associative and commutative
  ll n, K;
  vector<vi> anc; // remove & for edge queries or if instance lives longer
  vector<vector<T>> part;
  vi dep;
  // NODES
  PathQueries(const vector<vi>& g, vector<T>& vals)
      : n(g.size()), K(64 - __builtin_clzll(n)), anc(K, vi(n)), part(K, vector<T>(n, NEUT)), dep(n) {
  // EDGES
  // PathQueries(vector<vector<pair<ll, T>>> &_g)
  //     : n(_g.size()), K(64 - __builtin_clzll(n)), anc(K, vi(n)), part(K, vector<T>(n, NEUT)), dep(n) {
  //   vector<vi> g(n);
  //   fore(u, 0, n) for (auto [v, data] : _g[u]) g[u].pb(v);
    anc[0][0] = -1;
    vi s = {0};
    while (!s.empty()) {
      ll u = s.back();
      s.pop_back();
      for (ll v : g[u]) {
        if (v != anc[0][u]) {
          anc[0][v] = u, dep[v] = dep[u] + 1, s.push_back(v);
        }
      }
    }
    part[0] = vals;
    // fore(u, 0, n) for (auto [v, data] : _g[u]) part[0][dep[u] > dep[v] ? u : v] = data; // EDGES
    fore(k, 1, K) fore(v, 0, n) {
      if (anc[k - 1][v] != -1) {
        anc[k][v] = anc[k - 1][anc[k - 1][v]],
        part[k][v] = op(part[k - 1][v], part[k - 1][anc[k - 1][v]]);
      } else {
        anc[k][v] = -1;
      }
    }
  }
  T query(ll u, ll v) {
    if (dep[u] < dep[v])
      swap(u, v);
    T ans = NEUT;
    for (ll k = K - 1; k >= 0; k--)
      if (dep[u] >= dep[v] + (1 << k))
        ans = op(ans, part[k][u]), u = anc[k][u];
    // if (u == v) return ans; // edge queries
    if (u == v)
      return op(ans, part[0][u]); // node queries
    for (ll k = K - 1; k >= 0; k--)
      if (anc[k][u] != anc[k][v])
        ans = op(ans, op(part[k][u], part[k][v])), u = anc[k][u], v = anc[k][v];
    ans = op(ans, op(part[0][u], part[0][v]));
    // return ans; // edge queries
    return op(ans, part[0][anc[0][u]]); // node queries
  }
};
