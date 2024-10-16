/**
 * Author: Simon Lindholm
 * Date: 2019-12-28
 * License: CC0
 * Source: https://github.com/hoke-t/tamu-kactl/blob/master/content/data-structures/MoQueries.h
 * Description: Answer interval or tree path queries by finding an approximate TSP through the queries,
 * and moving from one query to the next by adding/removing points at the ends.
 * If values are on tree edges, change \texttt{step} to add/remove the edge $(a, c)$ and remove the initial \texttt{add} call (but keep \texttt{in}).
 * Time: O(N \sqrt Q)
 * Status: stress-tested
 */
#pragma once

void add(ll ind, ll end) {} // add a[ind] (end = 0 or 1)
void del(ll ind, ll end) {} // remove a[ind]
ll calc() { return 0; } // compute current answer

vi mo(vector<ii> Q) {
	ll L = 0, R = 0, blk = 350; // ~N/sqrt(Q)
	vi s(SZ(Q)), res = s;
#define K(x) ii(x.fst/blk, x.snd ^ -(x.fst/blk & 1))
	iota(ALL(s), 0);
	sort(ALL(s), [&](ll s, ll t) { return K(Q[s]) < K(Q[t]); });
	for (ll qi : s) {
		ii q = Q[qi];
		while (L > q.fst) add(--L, 0);
		while (R < q.snd) add(R++, 1);
		while (L < q.fst) del(L++, 0);
		while (R > q.snd) del(--R, 1);
		res[qi] = calc();
	}
	return res;
}

vi moTree(vector<array<ll, 2>> Q, vector<vi>& ed, ll root=0) {
	ll N = SZ(ed), pos[2] = {}, blk = 350; // ~N/sqrt(Q)
	vi s(SZ(Q)), res = s, I(N), L(N), R(N), in(N), par(N);
	add(0, 0), in[0] = 1;
	auto dfs = [&](ll x, ll p, ll dep, auto&& f) -> void {
		par[x] = p;
		L[x] = N;
		if (dep) I[x] = N++;
		for (ll y : ed[x]) if (y != p) f(y, x, !dep, f);
		if (!dep) I[x] = N++;
		R[x] = N;
	};
	dfs(root, -1, 0, dfs);
#define K(x) ii(I[x[0]] / blk, I[x[1]] ^ -(I[x[0]] / blk & 1))
	iota(ALL(s), 0);
	sort(ALL(s), [&](ll s, ll t) { return K(Q[s]) < K(Q[t]); });
	for (ll qi : s) fore(end,0,2) {
		ll &a = pos[end], b = Q[qi][end], i = 0;
#define step(c) { if (in[c]) { del(a, end); in[a] = 0; } \
                  else { add(c, end); in[c] = 1; } a = c; }
		while (!(L[b] <= L[a] && R[a] <= R[b]))
			I[i++] = b, b = par[b];
		while (a != b) step(par[a]);
		while (i--) step(I[i]);
		if (end) res[qi] = calc();
	}
	return res;
}
