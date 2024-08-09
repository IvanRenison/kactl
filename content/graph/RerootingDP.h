/**
 * Author: Pietro Palombini
 * Date: 2024-07-06
 * License: CC0
 * Source: https://codeforces.com/blog/entry/124286
 *
 * Description: \texttt{finalize} is applied before returning
 * answer for \texttt{node} if \texttt{g[node][p\_ei]} was its
 * parent; usually identity function.
 *
 * Let accumulated$(p, S) = a$ such that \texttt{finalize(a,
 * ...)} is the answer for $p$ if $S$ were its only children.
 * \texttt{merge} should, given accumulated$(p, S)$ and the
 * answer for \texttt{g[p][ei]}, compute
 * accumulated$(p,S\cup\{\texttt{g[p][ei]}\})$ in
 * \texttt{p\_ans}.
 *
 * \texttt{neuts[p]} should be accumulated$(p, \varnothing)$.
 *
 * Returns a tuple $(\texttt{rootDP}, \texttt{fdp},
 * \texttt{bdp})$ where \texttt{rootDP[v]} is the answer for
 * the whole tree with \texttt{v} as root, \texttt{fdp[v]} is
 * the answer for \texttt{g[v][ei]} if \texttt{v} is the root,
 * and \texttt{bdp[v]} is the answer for \texttt{v} if
 * \texttt{g[v][ei]} is the root.
 * Time: Fast O(n \log n) assuming O(1) lambdas.
 * Status: stress-tested
 */
#pragma once

// Equivalent to the following code, but for all roots:
// Data dfs(ll u, ll p) {
// 	Data res = neuts[u];
// 	fore(ei, 0, SZ(g[u])) if (g[u][ei] != p)
// 		acc(res, dfs(g[u][ei], u), u, ei);
// 	ll pid = find(ALL(g[u]), p) - begin(g[u]);
// 	return dp[u] = finalize(res, u, pid);
// }

struct Data {};
typedef vector<Data> vd;
auto reroot(vector<vi>& g, vd& neuts, auto& exclusive,
			auto& acc, auto& finalize) {
	ll n = SZ(g), pei;
	vd root_dp(n), exc(n), dp = neuts;
	vector<vd> fdp(n), bdp(n);
	vi o, p(n);
	o.reserve(n), o.pb(0);
	fore(i, 0, n) for (ll v : g[o[i]]) if (v != p[o[i]])
		p[v] = o[i], o.pb(v);
	for (ll u : views::reverse(o)) {
		fore(ei, 0, SZ(g[u])) if (g[u][ei] == p[u]) pei = ei;
		else acc(dp[u], dp[g[u][ei]], u, ei);
		dp[u] = finalize(dp[u], u, u ? pei : -1);
	}
	for (ll u : o) {
		dp[p[u]] = dp[u];
		fdp[u].reserve(SZ(g[u])), bdp[u].reserve(SZ(g[u]));
		for (ll v : g[u]) fdp[u].pb(dp[v]);
		exclusive(exc, fdp[u], neuts[u], u);
		fore(i,0,SZ(fdp[u])) bdp[u].pb(finalize(exc[i], u, i));
		acc(exc[0], fdp[u][0], u, 0);
		root_dp[u] = finalize(n>1 ? exc[0] : neuts[u], u, -1);
		fore(i, 0, SZ(g[u])) dp[g[u][i]] = bdp[u][i];
	}
	return tuple(root_dp, fdp, bdp);
}

void usage() {
	auto finalize = [&](Data& a, ll node, ll p_ei) -> Data {
		return a;
	};
	auto acc = [&](Data& p_ans, const Data& child_ans, ll p,
				   ll ei) -> void { p_ans = Data{}; };
	auto exclusive = [&acc](vd&exc, vd&a, Data&ne, ll v) {
		ll d = SZ(a);
		fill(begin(exc), begin(exc) + d, ne);
		for (ll b = bit_width((unsigned)d) - 1; b >= 0; b--) {
			for (ll i = d-1; i >= 0; i--)exc[i] = exc[i >> 1];
			fore(i,0,d-(d&!b))acc(exc[(i>>b)^1],a[i],v,i);
		}
	};
}
