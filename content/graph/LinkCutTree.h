/**
 * Author: Iván Renison
 * Date: 2024-05-23
 * Source: https://github.com/ngthanhtrung23/ACM_Notebook_new/blob/master/DataStructure/LinkCutTree.h
 * Description:
 * Time: All operations take amortized O(\log N).
 * Status: Untested
 */
#pragma once

typedef ll T;
T f(T a, T b) { return a + b; } // associative & commutative
T neg(T a, T b) { return a - b; } // Inverse of f
// (neg only is for subtree queries)
struct SplayTree {
	struct Node {
		array<ll, 2> child = {0, 0};
		ll parent = 0;
		array<T, 2> path;
		T self, sub, vir;
		bool reverse = false;
	};
	vector<Node> nodes;

	SplayTree(ll n) : nodes(n + 1) {}

	void splay(ll x) {
		for (pushDown(x); ~getDirection(x); ) {
			ll y = nodes[x].parent, z = nodes[y].parent;
			pushDown(z), pushDown(y), pushDown(x);
			ll dx = getDirection(x), dy = getDirection(y);
			if (~dy) rotate(dx != dy ? x : y);
			rotate(x);
		}
	}
	ll getDirection(ll x) {
		ll p = nodes[x].parent;
		if (!p) return -1;
		if (nodes[p].child[0] == x) return 0;
		return nodes[p].child[1] == x ? 1 : -1;
	}
	void rotate(ll x) {
		ll y = nodes[x].parent, dx = getDirection(x);
		ll z = nodes[y].parent, dy = getDirection(y);
		setChild(y, nodes[x].child[!dx], dx), setChild(x, y, !dx);
		if (~dy) setChild(z, x, dy);
		nodes[x].parent = z;
	}
	void pushDown(ll x) {
		if (!x) return;
		if (nodes[x].reverse) {
			auto [l, r] = nodes[x].child;
			nodes[l].reverse ^= 1, nodes[r].reverse ^= 1;
			swap(nodes[x].child[0], nodes[x].child[1]);
			swap(nodes[x].path[0], nodes[x].path[1]);
			nodes[x].reverse = false;
		}
	}
	void pushUp(ll x) {
		auto [l, r] = nodes[x].child;
		pushDown(l), pushDown(r);
		nodes[x].path[0] =
			f(f(nodes[l].path[0], nodes[x].self), nodes[r].path[0]);
		nodes[x].path[1] =
			f(f(nodes[r].path[1], nodes[x].self), nodes[l].path[1]);
		nodes[x].sub = f(
			f(f(nodes[x].vir, nodes[l].sub), nodes[r].sub),
			nodes[x].self);
	}
	void setChild(ll x, ll y, ll dir) {
		nodes[x].child[dir] = y;
		nodes[y].parent = x;
		pushUp(x);
	}
};

struct LinkCut : SplayTree {
	LinkCut(ll n) : SplayTree(n) {}

	bool is_connected(ll u, ll v) {
		return LCA(u, v) > 0;
	}
	void link(ll u, ll v) { // Add edge
		reroot(u), access(v);
		nodes[v].vir = f(nodes[v].vir, nodes[u].sub);
		nodes[u].parent = v;
		pushUp(v);
	}
	void cut(ll u, ll v) { // Remove edge
		reroot(u), access(v);
		nodes[v].child[0] = nodes[u].parent = 0;
		pushUp(v);
	}
	// Returns 0 if u and v are not connected
	ll LCA(ll u, ll v) {
		if (u == v) return u;
		access(u);
		ll ret = access(v);
		return nodes[u].parent ? ret : 0;
	}
	T getPath(ll u, ll v) { // query on path
		reroot(u), access(v);
		return nodes[v].path[1];
	}
	void set(ll u, T val) { // Update value of node
		access(u);
		nodes[u].self = val;
		pushUp(u);
	}
	T get(ll u) { // Queries element
		return nodes[u].self;
	}
	T getSubtree(ll u, ll v) { // Queries subtree
		reroot(v); access(u);
		return f(nodes[u].vir, nodes[u].self);
	}
	void reroot(ll x) {
		access(x);
		nodes[x].reverse ^= 1;
		pushDown(x);
	}
	ll access(ll x) {
		ll u = x, v = 0;
		for (; u; v = u, u = nodes[u].parent) {
			splay(u);
			ll& ov = nodes[u].child[1];
			nodes[u].vir = f(nodes[u].vir, nodes[ov].sub);
#ifdef SUBTREE_QUERIES
			nodes[u].vir = neg(nodes[u].vir, nodes[v].sub);
#endif
			ov = v; pushUp(u);
		}
		return splay(x), v;
	}
};
