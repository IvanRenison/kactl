
#include "../../content/data-structures/SegmentTree.h"

typedef vector<ii> vii;

struct Node {
	ll d, par, val, chain = -1, pos = -1;
};

struct Chain {
	ll par, val;
	vi nodes;
	Tree tree;
};

struct HLD {
	typedef ll T;
	const T LOW = -(1<<29);
	void f(T& a, T b) { a = max(a, b); }

	vector<Node> V;
	vector<Chain> C;

	HLD(vector<vii>& g) : V(SZ(g)) {
		dfs(0, -1, g, 0);
		for(auto &c: C) {
			c.tree = {SZ(c.nodes), 0};
			for (ll ni : c.nodes)
				c.tree.upd(V[ni].pos, V[ni].val);
		}
	}

	void upd(ll node, T val) {
		Node& n = V[node]; n.val = val;
		if (n.chain != -1) C[n.chain].tree.upd(n.pos, val);
	}

	ll pard(Node& nod) {
		if (nod.par == -1) return -1;
		return V[nod.chain == -1 ? nod.par : C[nod.chain].par].d;
	}

	// query all *edges* between n1, n2
	pair<T, ll> query(ll i1, ll i2) {
		T ans = LOW;
		while(i1 != i2) {
			Node n1 = V[i1], n2 = V[i2];
			if (n1.chain != -1 && n1.chain == n2.chain) {
				ll lo = n1.pos, hi = n2.pos;
				if (lo > hi) swap(lo, hi);
				f(ans, C[n1.chain].tree.query(lo, hi));
				i1 = i2 = C[n1.chain].nodes[hi];
			} else {
				if (pard(n1) < pard(n2))
					n1 = n2, swap(i1, i2);
				if (n1.chain == -1)
					f(ans, n1.val), i1 = n1.par;
				else {
					Chain& c = C[n1.chain];
					f(ans, n1.pos ? c.tree.query(n1.pos, SZ(c.nodes))
					              : c.tree.s[1]);
					i1 = c.par;
				}
			}
		}
		return make_pair(ans, i1);
	}

	// query all *nodes* between n1, n2
	pair<T, ll> query2(ll i1, ll i2) {
		pair<T, ll> ans = query(i1, i2);
		f(ans.fst, V[ans.snd].val);
		return ans;
	}

	ii dfs(ll at, ll par, vector<vii>& g, ll d) {
		V[at].d = d; V[at].par = par;
		ll sum = 1, ch, nod, sz;
		tuple<ll,ll,ll> mx(-1,-1,-1);
		for(auto &e: g[at]){
			if (e.fst == par) continue;
			tie(sz, ch) = dfs(e.fst, at, g, d+1);
			V[e.fst].val = e.snd;
			sum += sz;
			mx = max(mx, make_tuple(sz, e.fst, ch));
		}
		tie(sz, nod, ch) = mx;
		if (2*sz < sum) return {sum, -1};
		if (ch == -1) { ch = SZ(C); C.pb({}); }
		V[nod].pos = SZ(C[ch].nodes);
		V[nod].chain = ch;
		C[ch].par = at;
		C[ch].nodes.pb(nod);
		return {sum, ch};
	}
};
