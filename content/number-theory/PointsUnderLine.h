/**
 * Author: Unknown
 * Date: 2024-11-05
 * Source: notebook el vasito
 * License: CC0
 * Description: Given $a,b > 0$, \texttt{f} returns the number
 * of lattice points $(x, y)$ such that $ax + by \leq c$ and
 * $x, y > 0$, and \texttt{g} returns the number of lattice
 * points $(x, y)$ such that $ax + by \leq c$, $0 < x \leq X$
 * and $0 < y \leq Y$.
 * Status: untested
 */

#pragma once

ll f(ll a, ll b, ll c) {
  if (c <= 0) return 0;
  if (a < b) swap(a, b);
  ll m = c / a;
  if (a == b) return m * (m - 1) / 2;
  ll k = (a - 1) / b, h = (c - a * m) / b;
  return f(b, a - b*k, c - b*(k*m + h)) + k*m*(m - 1)/2 + m*h;
}
ll g(ll a, ll b, ll c, ll X, ll Y) {
  if (a * X + b * Y <= c) return X * Y;
  return f(a,b,c)-f(a,b,c-a*X)-f(a,b,c-b*Y)+f(a,b,c-a*X-b*Y);
}
