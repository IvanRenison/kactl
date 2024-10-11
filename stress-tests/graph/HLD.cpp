#include "../utilities/template.h"
#include "../utilities/genTree.h"

#include "../../content/graph/HLD.h"

struct bruteforce { // values in nodes
	vector<vi> tree;
	vector<T> vals;
	vi pars;
	void root(ll cur, ll p = -1) {
		pars[cur] = p;
		for (auto i: tree[cur]) {
			if (i != p) root(i, cur);
		}
	}
	bruteforce(vector<vi> _tree): tree(_tree), vals(SZ(tree)), pars(SZ(tree)) {
		root(0);
	}
	bool dfsModify(ll cur, ll target, L val, ll p=-1) {
		if (cur == target) {
			vals[cur] = apply(vals[cur], val, 0, 1);
			return true;
		}
		bool alongPath = false;
		for (auto i : tree[cur]) {
			if (i == p) continue;
			alongPath |= dfsModify(i, target, val, cur);
		}
		if (alongPath) vals[cur] = apply(vals[cur], val, 0, 1);
		return alongPath;
	}
	void updPath(ll a, ll b, L val) {
		dfsModify(a, b, val);
	}

	pair<T, bool> dfsQuery(ll cur, ll target, ll p = -1) {
		if (cur == target) {
			return {vals[cur], true};
		}
		T res = tneut;
		bool alongPath = false;
		for (auto i : tree[cur]) {
			if (i == p) continue;
			auto [subres, subalongPath] = dfsQuery(i, target, cur);
			if (subalongPath) res = f(res, subres);
			alongPath |= subalongPath;
		}
		if (alongPath) {
			return {f(res, vals[cur]), true};
		}
		return {res, false};
	}
	T queryPath(ll a, ll b) {
		return dfsQuery(a, b).fst;
	}
	T dfsSubtree(ll cur, ll p) {
		ll res = vals[cur];
		for (auto i : tree[cur]) {
			if (i != p)
				res = f(res, dfsSubtree(i, cur));
		}
		return res;
	}
	T querySubtree(ll a) {
		return dfsSubtree(a, pars[a]);
	}
};

void testAgainstBrute(ll n, ll iters, ll queries) {
	for (ll trees = 0; trees < iters; trees++) {
		vector<ii> graph = genRandomTree(n);
		vector<vi> tree1(n);
		for (auto [u, v] : graph) {
			tree1[u].pb(v), tree1[v].pb(u);
		}
		HLD<false> hld(tree1);
		bruteforce hld2(tree1);
		for (ll itr = 0; itr < queries; itr++) {
			ll rng = rand() % 3;
			if (rng == 0) {
				ll a = rand() % n;
				ll b = rand() % n;
				ll val = rand() % 10;
				hld.updPath(a, b, val);
				hld2.updPath(a, b, val);
			} else if (rng == 1) {
				ll a = rand() % n;
				ll b = rand() % n;
				hld.queryPath(a, b);
				hld2.queryPath(a, b);
				ll ans = hld.queryPath(a, b);
				ll ans2 = hld2.queryPath(a, b);
				assert(ans == ans2);
			} else if (rng == 2) {
				ll a = rand() % n;
				assert(hld.querySubtree(a) == hld2.querySubtree(a));
			}
		}
	}
}

int main() {
	srand(2);
	testAgainstBrute(5, 1000, 10000);
	testAgainstBrute(1000, 100, 100);
	cout << "Tests passed!" << endl;
}
