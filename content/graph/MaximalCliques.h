/**
 * Author: Simon Lindholm
 * Date: 2018-07-18
 * License: CC0
 * Source: https://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm
 * Description: Runs a callback for all maximal cliques in a graph (given as a
 * symmetric bitset matrix; self-edges not allowed). Callback is given a bitset
 * representing the maximal clique.
 * Time: O(3^{n/3}), much faster for sparse graphs
 * Status: stress-tested
 */
#pragma once
/// Possible optimization: on the top-most
/// recursion level, ignore 'cands', and go through nodes in order of increasing
/// degree, where degrees go down as nodes are removed.
/// (mostly irrelevant given MaximumClique)

typedef bitset<128> B;
void cliques(vector<B>& eds, auto&& f, B P=-1, B X=0, B R=0) {
	if (!P.any()) { if (!X.any()) f(R); return; }
	ll q = (P | X)._Find_first();
	B cands = P & ~eds[q];
	fore(i,0,SZ(eds)) if (cands[i])
		R[i] = 1, cliques(eds, f, P & eds[i], X & eds[i], R),
		R[i] = P[i] = 0, X[i] = 1;
}
