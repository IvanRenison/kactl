/**
 * Author: Johan Sannemo
 * Date: 2015-02-06
 * License: CC0
 * Source: Folklore
 * Description: Calculate power of two jumps in a tree,
 * to support fast upward jumps and LCAs.
 * Assumes the root node points to itself.
 * Time: construction $O(N \log N)$, queries $O(\log N)$
 * Status: stress-tested directly and via LCA.cpp
 */
#pragma once

vector<vi> treeJump(vi& P){
	ll d = bit_width((unsigned)SZ(P));
	vector<vi> jmp(d, P);
	fore(i,1,d)fore(j,0,SZ(P)) jmp[i][j] = jmp[i-1][jmp[i-1][j]];
	return jmp;
}

ll jmp(vector<vi>& tbl, ll nod, ll steps) {
	fore(i,0,SZ(tbl)) if (steps & (1<<i)) nod = tbl[i][nod];
	return nod;
}

ll lca(vector<vi>& tbl, vi& depth, ll a, ll b) {
	if (depth[a] < depth[b]) swap(a, b);
	a = jmp(tbl, a, depth[a] - depth[b]);
	if (a == b) return a;
	for (ll i = SZ(tbl); i--;) {
		ll c = tbl[i][a], d = tbl[i][b];
		if (c != d) a = c, b = d;
	}
	return tbl[0][a];
}
