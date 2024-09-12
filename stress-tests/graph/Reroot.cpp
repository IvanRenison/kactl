#include "../utilities/template.h"
#include "../utilities/genTree.h"

/// content/graph/Reroot.h
/// START diff
struct Data {
	ll x;
	bool operator==(const Data &o) const { return x == o.x; }
};
/// END diff
typedef vector<Data> vd;

struct Reroot {
	ll n;
	vector<vi>& g;
	vd& neuts;
	/// START diff
	vi &ndata;
	map<ii, ll> &edata;
	ll MOD;
	/// END diff
	Data finalize(const Data& a, ll p, ll ei) {
		/// START diff
		return {a.x * ndata[p] % (ei < 0 ? MOD : abs(MOD - g[p][ei]) + 2)};
		/// END diff
	}
	void acc(Data& p_ans, const Data& child_ans, ll p, ll ei) {
		/// START diff
		p_ans = {p_ans.x + (child_ans.x ^ edata.at(minmax(p, g[p][ei])))};
		/// END diff
	}

	vd root_dp;
	vector<vd> fdp, bdp;
	/// START diff
	Reroot(vector<vi>& g, vd& neuts, vi& ndata, map<ii, ll>& edata, ll MOD)
		: n(SZ(g)),
		  g(g),
		  neuts(neuts),
		  ndata(ndata),
		  edata(edata),
		  MOD(MOD),
		  root_dp(n),
		  fdp(n),
		  bdp(n) {}
	/// END diff
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
/// END content

/// content/graph/RerootLinear.h
struct RerootLinear : Reroot {
	/// START diff
	using Reroot::Reroot;
	/// END diff
	Data merge(const Data& a, const Data& b, ll p) {
		/// START diff
		return {a.x + b.x - neuts[p].x};
		/// END diff
	}
	Data extend(const Data& a, ll p, ll ei) {
		/// START diff
		return {(a.x ^ edata[minmax(p, g[p][ei])]) + neuts[p].x};
		/// END diff
	}
	void acc(Data& p_ans, const Data& child_ans, ll p, ll ei) {
		p_ans = merge(p_ans, extend(child_ans, p, ei), p);
	}
	void ex(vd& e, vd& a, Data& ne, ll v) {
		ll d = SZ(a);
		vd p(d + 1, ne), s(d + 1, ne);
		fore(i,0,d) p[i+1] = merge(p[i], a[i]=extend(a[i],v,i), v);
		for(ll i=d-1;i>=0;i--) s[i] = merge(a[i], s[i + 1], v);
		fore(i, 0, d) e[i] = merge(p[i], s[i + 1], v);
	}
};
/// END content

/// content/graph/RerootInv.h
struct RerootInv : Reroot {
	/// START diff
	using Reroot::Reroot;
	/// END diff
	void unacc(Data& ans, const Data& child_ans, ll p, ll ei) {
		/// START diff
		ans = {ans.x - (child_ans.x ^ edata.at(minmax(p, g[p][ei])))};
		/// END diff
	}
	void ex(vd& e, vd& a, Data& ne, ll v) {
		ll d = SZ(a);
		Data b = ne;
		fore(i, 0, d) acc(b, a[i], v, i);
		fill(begin(e), begin(e) + d, b);
		fore(i, 0, d) unacc(e[i], a[i], v, i);
	}
};
/// END content

vd treeDP(const vector<vi> &g, const vd &neuts, auto &acc, auto &finalize,
		  ll root) {
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
	vd root_dp(n);
	map<ii, Data> edgeDP;
	fore(root, 0, n) {
		auto dp = treeDP(g, neuts, acc, finalize, root);
		root_dp[root] = dp[root];
		for (ll v : g[root]) { edgeDP[{root, v}] = dp[v]; }
	}
	vector<vd> fdp(n), bdp(n);
	fore(i, 0, n) {
		for (ll v : g[i]) {
			fdp[i].pb(edgeDP[{i, v}]);
			bdp[i].pb(edgeDP[{v, i}]);
		}
	}
	return tuple(root_dp, fdp, bdp);
}

auto genCase(ll n) {
	vector<vi> g(n);
	auto edges = genRandomTree(n);
	for (auto &[u, v] : edges) {
		g[u].pb(v);
		g[v].pb(u);
	}
	map<ii, ll> edata;
	fore(i, 0, n) for (ll v : g[i]) edata[minmax(i, v)] =
		rand() % 2'000'001 - 1'000'000;
	vi ndata(n);
	vd neut(n);
	fore(i, 0, n) ndata[i] = rand() % 2'000'001 - 1'000'000,
				  neut[i] = {rand() % 2'000'001 - 1'000'000};
	ll MOD = rand() % 10'000'000 + 2;
	return tuple(g, edata, ndata, neut, MOD);
}

ll TIMES = 100, MAXN = 1000;

int main() {
	srand(2);
	fore(_, 0, TIMES) {
		ll n = rand() % MAXN + 1;
		auto [g, edata, ndata, neuts, MOD] = genCase(n);
		Reroot reroot(g, neuts, ndata, edata, MOD);
		RerootLinear rerootLinear(g, neuts, ndata, edata, MOD);
		RerootInv rerootInv(g, neuts, ndata, edata, MOD);
		reroot.reroot();
		rerootLinear.reroot();
		rerootInv.reroot();

		auto finalize = [&](const Data &a, ll p, ll ei) -> Data {
			return {a.x * ndata[p] % (ei < 0 ? MOD : abs(MOD - g[p][ei]) + 2)};
		};
		auto acc = [&](Data &p_ans, const Data &child_ans, ll p, ll ei) {
			p_ans = {p_ans.x + (child_ans.x ^ edata.at(minmax(p, g[p][ei])))};
		};
		auto [root_dp, fdp, bdp] = slow(g, neuts, acc, finalize);
		assert(reroot.root_dp == root_dp);
		assert(rerootLinear.root_dp == root_dp);
		assert(rerootInv.root_dp == root_dp);
		assert(reroot.fdp == fdp);
		assert(rerootLinear.fdp == fdp);
		assert(rerootInv.fdp == fdp);
		assert(reroot.bdp == bdp);
		assert(rerootLinear.bdp == bdp);
		assert(rerootInv.bdp == bdp);
	}
}
