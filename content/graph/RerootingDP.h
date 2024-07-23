/**
 * Author: Pietro Palombini
 * Date: 2024-07-06
 * License: CC0
 * Source: https://codeforces.com/blog/entry/124286
 * Description: Rerooting DP, with tree versions of `exclusive`
 * function. Define the lambdas needed by the used version.
 * Time: Fast O(n \log n), slow O(n) and fast O(n)
 * respectively, assuming O(1) lambdas.
 * Status: stress-tested
 */
#pragma once

struct Data {};
typedef vector<Data> vd;
// rootDP[v] = answer for the whole tree with v as root
// fdp[v][ei] = answer for g[v][ei] if v is the root
// bdp[v][ei] = answer for v if g[v][ei] is the root
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

// accumulated(p, S) = a such that finalize(a, ...) is the
// answer for p if S were its only children
void solve() {
	// applied before returning answer for node if
	// g[node][p_ei] was its parent; generally just identity
	// function
	auto finalize = [&](Data& a, ll node, ll p_ei) -> Data {
		return a;
	};
	// given accumulated(p, S) and accumulated(p, T), with S
	// and T disjoint, compute accumulated(p, S \cup T)
	auto merge = [](const Data& a, const Data& b, ll p)->Data {
		return Data{};
	};
	// given the answer for g[p][ei], compute b such that
	// merge(neuts[p], b, p) = accumulated(p, {g[p][ei]})
	auto extend = [](const Data& a, ll p, ll ei) -> Data {
		return Data{};
	};
	// given accumulated(p, S) and the answer for g[p][ei],
	// compute accumulated(p, S \cup {g[p][ei]}). Mutate p_ans.
	// leave as-is for v2
	auto acc = [&](Data& p_ans, const Data& child_ans, ll p,
				   ll ei) -> void {
		p_ans = merge(p_ans, extend(child_ans, p, ei), p);
	};
	auto v1 = [&acc](vd& exc, vd& a, Data& neut, ll node) {
		ll d = SZ(a);
		fill(begin(exc), begin(exc) + d, neut);
		for (ll b = bit_width((unsigned)d) - 1; b >= 0; b--) {
			for (ll i = d-1; i >= 0; i--)exc[i] = exc[i >> 1];
			fore(i,0,d-(d&!b))acc(exc[(i>>b)^1],a[i],node,i);
		}
	};
	auto v2 = [&extend,&merge](vd&exc, vd a, Data& neut,ll v) {
		ll d = SZ(a);
		vd p(d + 1, neut), s(d + 1, neut);
		fore(i,0,d)p[i+1]=merge(p[i],a[i]=extend(a[i],v,i),v);
		for (ll i=d-1;i>=0;i--) s[i] = merge(a[i],s[i+1],v);
		fore(i, 0, d) exc[i] = merge(p[i], s[i + 1], v);
	};

	// unacc(Data& ans, const Data& child_ans, ll p, ll ei) ->
	// void given accumulated(p, g[p]) and the answer for
	// g[p][ei], compute accumulated(p, S). Mutate ans
	auto unacc = [&](Data& ans, const Data& child_ans, ll p,
					 ll ei) -> void {};
	auto v3 = [&acc,&unacc](vd& exc,vd& a,Data& neut,ll node) {
		ll d = SZ(a);
		Data b = neut;
		fore(i, 0, d) acc(b, a[i], node, i);
		fill(begin(exc), begin(exc) + d, b);
		fore(i, 0, d) unacc(exc[i], a[i], node, i);
	};
}
