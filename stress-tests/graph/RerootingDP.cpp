#include "../utilities/template.h"
#include "../utilities/genTree.h"

struct Data {
	ll x;
	bool operator==(const Data &o) const { return x == o.x; }
};
typedef vector<Data> vd;
auto reroot(vector<vi>& g, vd& neuts, auto& exclusive,
			auto& acc, auto& finalize) {
	ll n = SZ(g), pei;
	vd root_dp(n), exc(n), dp = neuts;
	vector<vd> fdp(n), bdp(n);
	if(n==1) return tuple(vd{finalize(neuts[0],0,-1)},fdp,bdp);
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

vd treeDP(const vector<vi> &g, const vd &neuts, auto &acc, auto &finalize, ll root) {
	ll n = SZ(g);
	vd dp(n);
	function<Data(ll, ll)> dfs = [&](ll u, ll p) {
		Data res = neuts[u];
		ll pid = -1;
		fore(ei, 0, SZ(g[u])) if (g[u][ei] == p) pid = ei;
		else acc(res, dfs(g[u][ei], u), u, ei);
		return dp[u] = finalize(res, u, pid);
	};
	dfs(root, -1);
	return dp;
}

auto slow(const vector<vi> &g, const vd &neuts, auto &acc, auto &finalize) {
	ll n = SZ(g);
	vd rootDP(n);
	map<ii, Data> edgeDP;
	fore(root, 0, n) {
		auto dp = treeDP(g, neuts, acc, finalize, root);
		rootDP[root] = dp[root];
		for (ll v : g[root]) {
			edgeDP[{root, v}] = dp[v];
		}
	}
	vector<vd> fdp(n), bdp(n);
	fore(i, 0, n) {
		for (ll v : g[i]) {
			fdp[i].pb(edgeDP[{i, v}]);
			bdp[i].pb(edgeDP[{v, i}]);
		}
	}
	return tuple(rootDP, fdp, bdp);
}

auto genCase(ll n) {
	vector<vi> g(n);
	auto edges = genRandomTree(n);
	for (auto &[u, v] : edges) {
		g[u].pb(v);
		g[v].pb(u);
	}
	map<ii, ll> edata;
	fore(i, 0, n) for (ll v : g[i]) edata[minmax(i, v)] = rand() % 2'000'001 - 1'000'000;
	vi ndata(n);
	vd neut(n);
	fore(i, 0, n) ndata[i] = rand() % 2'000'001 - 1'000'000,
				  neut[i] = {rand() % 2'000'001 - 1'000'000};
	ll MOD = rand() % 10'000'000 + 2;
	return tuple(g, edata, ndata, neut, MOD);
}

ll TIMES = 100,
   MAXN = 1000;

int main() {
	srand(2);
	fore(_, 0, TIMES) {
		ll n = rand() % MAXN + 1;
		auto [g, edata, ndata, neuts, MOD] = genCase(n);
		auto extend = [&](const Data &a, ll p, ll ei) -> Data {
			return {(a.x ^ edata[minmax(p, g[p][ei])]) + neuts[p].x};
			// return {a.x + neuts[p].x};
		};
		auto merge = [&](const Data &a, const Data &b, ll p) -> Data {
			return {a.x + b.x - neuts[p].x};
		};
		auto acc = [&](Data &p_ans, const Data &child_ans, ll p, ll ei) -> void {
			p_ans = {p_ans.x + (child_ans.x ^ edata.at(minmax(p, g[p][ei])))};
			// p_ans = {p_ans.x + child_ans.x};
		};
		auto unacc = [&](Data &ans, const Data &child_ans, ll p, ll ei) -> void {
			ans = {ans.x - (child_ans.x ^ edata.at(minmax(p, g[p][ei])))};
			// ans = {ans.x - child_ans.x};
		};
		auto finalize = [&](const Data &a, ll p, ll ei) -> Data {
			return {a.x * ndata[p] % (ei < 0 ? MOD : abs(MOD - g[p][ei]) + 2)};
			// return a;
		};
		auto v1 = [&acc](vd& exc, vd& a, Data& ne, ll v) {
			ll d = SZ(a);
			fill(begin(exc), begin(exc) + d, ne);
			for (ll b = bit_width((unsigned)d) - 1; b >= 0; b--) {
				for (ll i = d-1; i >= 0; i--)exc[i] = exc[i >> 1];
				fore(i,0,d-(d&!b))acc(exc[(i>>b)^1],a[i],v,i);
			}
		};
		auto v2 = [&extend,&merge](vd&exc, vd a, Data& ne,ll v) {
			ll d = SZ(a);
			vd p(d + 1, ne), s(d + 1, ne);
			fore(i,0,d)p[i+1]=merge(p[i],a[i]=extend(a[i],v,i),v);
			for (ll i=d-1;i>=0;i--) s[i] = merge(a[i],s[i+1],v);
			fore(i, 0, d) exc[i] = merge(p[i], s[i + 1], v);
		};
		auto v3 = [&acc, &unacc](vd& exc, vd& a, Data& ne, ll v) {
			ll d = SZ(a);
			Data b = ne;
			fore(i, 0, d) acc(b, a[i], v, i);
			fill(begin(exc), begin(exc) + d, b);
			fore(i, 0, d) unacc(exc[i], a[i], v, i);
		};
		auto [rootDP1, fdp1, bdp1] = reroot(g, neuts, v1, acc, finalize);
		auto [rootDP2, fdp2, bdp2] = reroot(g, neuts, v2, acc, finalize);
		auto [rootDP3, fdp3, bdp3] = reroot(g, neuts, v3, acc, finalize);
		auto [rootDP4, fdp4, bdp4] = slow(g, neuts, acc, finalize);
		assert(rootDP1 == rootDP4);
		assert(rootDP2 == rootDP4);
		assert(rootDP3 == rootDP4);
		assert(fdp1 == fdp4);
		assert(fdp2 == fdp4);
		assert(fdp3 == fdp4);
		assert(bdp1 == bdp4);
		assert(bdp2 == bdp4);
		assert(bdp3 == bdp4);
	}
}
