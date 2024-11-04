/**
 * Author: Pietro Palombini
 * Date: 2024-11-03
 * License: CC0
 * Source: https://codeforces.com/blog/entry/61203/
 * Description: Answer range queries offline using Hilbert
 * order or SQRT decomposition. For Hilbert, change lines with
 * \texttt{SQ} for commented lines. Define \texttt{add},
 * \texttt{rem}, \texttt{calc}, add necessary fields to
 * \texttt{MoQueries}, instantiate, and call \texttt{solve}.
 * Queries are given as pairs $[l, r)$. \texttt{end} is true if
 * the element should be added/removed from the end of the
 * range.
 * Time: Fast O(T N \sqrt Q + CQ) where T is the cost of
 * \texttt{add} and \texttt{rem} and C is the cost of
 * \texttt{calc}.
 */

#pragma once

constexpr ll B = 447; // ~N/sqrt(Q)
struct MoQueries {
  typedef ll T;
  void add(ll pos, bool end) {}
  void rem(ll pos, bool end) {}
  T calc() { return 0; }
  ii k(ii &x){return ii(x.fst/B, x.snd ^ -(x.fst/B&1));}// SQ
  vector<T> solve(ll n, vector<ii> &qs) {
    ll l=0,r=0,q=SZ(qs); //,k=bit_width((unsigned)n), rx,ry;
    vi p(q); //,o(q);
    iota(ALL(p), 0);
  //fore(i, 0, q) {
  //  ll x = qs[i].fst, y = qs[i].snd;
  //  for (ll s = k; --s;) {
  //    rx=(x>>s)&1, ry=(y>>s)&1, o[i] += ((rx*3)^ry) << (2*s);
  //    if (!ry) {
  //      if (rx) x = (1 << k) - 1 - x, y = (1 << k) - 1 - y;
  //      swap(x, y);
  //    }
  //  }
  //}
  //sort(ALL(p), [&](ll i, ll j) { return o[i] < o[j]; });
    sort(ALL(p),[&](ll i,ll j){return k(qs[i])<k(qs[j]);});//SQ
    vector<T> res(q);
    for (ll i : p) {
      auto [ql, qr] = qs[i];
      while (l > ql) add(--l, 0);
      while (r < qr) add(r++, 1);
      while (l < ql) rem(l++, 0);
      while (r > qr) rem(--r, 1);
      res[i] = calc();
    }
    return res;
  }
};
