/**
 * Author: Pietro Palombini
 * Date: 2024-07-06
 * License: CC0
 * Source: https://codeforces.com/blog/entry/124286
 * Description: Usage: define \texttt{Data}, \texttt{finalize},
 * \texttt{acc} according to the definitions below, adding any
 * necessary fields to \texttt{Reroot}. Then instantiate, add
 * the data, and call \texttt{reroot()}.
 *
 * \texttt{finalize} is applied before returning
 * answer for \texttt{p} if \texttt{g[p][ei]} was its
 * parent; usually identity function.
 *
 * Let accumulated$(p, S) = a$ such that \texttt{finalize(a,
 * ...)} is the answer for $p$ if $S$ were its only children.
 * \texttt{acc} should, given accumulated$(p, S)$ and the
 * answer for \texttt{g[p][ei]}, compute
 * accumulated$(p,S\cup\{\texttt{g[p][ei]}\})$ in
 * \texttt{p\_ans}.
 *
 * \texttt{neuts[p]} should be accumulated$(p, \varnothing)$.
 *
 * The answers are \texttt{root\_dp[v]}, the answer for the
 * whole tree with \texttt{v} as root, \texttt{fdp[v][ei]}, the
 * answer for \texttt{g[v][ei]} if \texttt{v} is the root, and
 * \texttt{bdp[v][ei]}, the answer for \texttt{v} if
 * \texttt{g[v][ei]} is the root.
 *
 * Equivalent to the following code, but for all roots:
 *
 * \texttt{Data dfs(ll u, ll p) \{}
 *
 * \ \ \texttt{Data res = neuts[u];}
 *
 * \ \ \texttt{fore(ei, 0, SZ(g[u])) if (g[u][ei] != p)}
 *
 * \ \ \ \ \texttt{acc(res, dfs(g[u][ei], u), u, ei);}
 *
 * \ \ \texttt{ll pid = find(ALL(g[u]), p) - begin(g[u]);}
 *
 * \ \ \texttt{return dp[u] = finalize(res, u, pid);}
 *
 * \texttt{\}}
 * Time: Fast O(n \log n) assuming O(1) operations.
 * Status: stress-tested
 */

#pragma once

struct Data {};
typedef vector<Data> vd;

struct Reroot {
	ll n;
	vector<vi>& g;
	vd& neuts;

	Data finalize(const Data& a, ll p, ll ei) { return a; }
	void acc(Data& p_ans, const Data& child_ans, ll p, ll ei) {
		p_ans = Data{};
	}

	vd root_dp;
	vector<vd> fdp, bdp;
	Reroot(vector<vi>&g,vd&neuts)
		: n(SZ(g)), g(g), neuts(neuts), root_dp(n), fdp(n),bdp(n){}
	void reroot() {
		if (n==1) { root_dp[0]=finalize(neuts[0], 0, -1); return; }
		vd dp = neuts, e(n); vi o, p(n); o.reserve(n), o.pb(0);
		fore(i, 0, n) for (ll v : g[o[i]]) if (v != p[o[i]])
			p[v] = o[i], o.pb(v);
		for (ll u : views::reverse(o)) {
			ll pei = -1;
			fore(ei, 0, SZ(g[u])) if (g[u][ei] == p[u]) pei = ei;
			else acc(dp[u], dp[g[u][ei]], u, ei);
			dp[u] = finalize(dp[u], u, pei);
		}
		for (ll u : o) {
			dp[p[u]] = dp[u];
			fdp[u].reserve(SZ(g[u])), bdp[u].reserve(SZ(g[u]));
			for (ll v : g[u]) fdp[u].pb(dp[v]);
			ex(e, fdp[u], neuts[u], u);
			fore(i, 0, SZ(fdp[u])) bdp[u].pb(finalize(e[i], u, i));
			acc(e[0], fdp[u][0], u, 0);
			root_dp[u] = finalize(n > 1 ? e[0] : neuts[u], u, -1);
			fore(i, 0, SZ(g[u])) dp[g[u][i]] = bdp[u][i];
		}
	}
	void ex(vd& e, vd& a, Data& ne, ll v) {
		ll d = SZ(a); fill(begin(e), begin(e) + d, ne);
		for (ll b = bit_width((unsigned)d) - 1; b >= 0; b--) {
			for (ll i = d - 1; i >= 0; i--)
				e[i] = e[i >> 1];
			fore(i, 0, d - (d & !b))
				acc(e[(i >> b) ^ 1], a[i], v, i);
		}
	}
};
