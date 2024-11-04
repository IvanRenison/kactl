/**
 * Author: Simon Lindholm
 * Date: 2019-12-28
 * License: CC0
 * Source: https://github.com/hoke-t/tamu-kactl/blob/master/content/data-structures/MoQueries.h
 * Description: Answer tree path queries by finding an
 * approximate TSP through the queries, and moving from one
 * query to the next by adding/removing points at the ends. If
 * values are on tree edges, change \texttt{step} to add/remove
 * the edge $(a, c)$ and remove the initial \texttt{add} call
 * (but keep \texttt{in}).
 * Time: O(N \sqrt Q)
 * Status: stress-tested
 */
#pragma once

constexpr ll B = 447; // ~N/sqrt(Q)
struct MoTree {
	typedef ll T;
	void add(ll pos, ll end) {}
	void del(ll pos, ll end) {}
	T calc() { return 0; }
	vector<T> solve(vector<ii> Q, vector<vi>& g, ll root=0) {
		ll N = SZ(g), pos[2] = {};
		vi s(SZ(Q)), I(N), L(N), R(N), in(N), par(N);
		vector<T> res(SZ(Q));
		add(0, 0), in[0] = 1;
		auto dfs = [&](ll x, ll p, ll dep, auto&& f) -> void {
			par[x] = p, L[x] = dep ? I[x] = N++ : N;
			for (ll y : g[x]) if (y != p) f(y, x, !dep, f);
			R[x] = !dep ? I[x] = ++N : N;
		};
		dfs(root, -1, 0, dfs);
		#define K(x) ii(I[x.fst]/B, I[x.snd] ^ -(I[x.fst]/B & 1))
		iota(ALL(s), 0);
		sort(ALL(s), [&](ll s, ll t) {return K(Q[s]) < K(Q[t]);});
		for (ll qi : s) fore(e,0,2) {
			ll &a = pos[e], b = e ? Q[qi].fst : Q[qi].snd, i = 0;
#define step(c)in[c]?(del(a,e),in[a]=0):(add(c,e),in[c]=1),a=c;
			while(!(L[b] <= L[a] && R[a] <= R[b])) I[i++]=b,b=par[b];
			while (a != b) step(par[a]);
			while (i--) step(I[i]);
			if (e) res[qi] = calc();
		}
		return res;
	}
};
