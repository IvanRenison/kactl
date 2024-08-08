/**
 * Author: Johan Sannemo
 * License: CC0
 * Description: Compute indices of smallest set of intervals covering another interval.
 * Intervals should be [inclusive, exclusive). To support [inclusive, inclusive],
 * change (A) to add \texttt{|| R.empty()}. Returns empty set on failure (or if G is empty).
 * Time: O(N \log N)
 * Status: Tested on kattis:intervalcover
 */
#pragma once

template<class T>
vi cover(pair<T, T> G, vector<pair<T, T>> I) {
	vi S(SZ(I)), R;
	iota(ALL(S), 0);
	sort(ALL(S), [&](ll a, ll b) { return I[a] < I[b]; });
	T cur = G.fst;
	ll at = 0;
	while (cur < G.snd) { // (A)
		pair<T, ll> mx = make_pair(cur, -1);
		while (at < SZ(I) && I[S[at]].fst <= cur) {
			mx = max(mx, make_pair(I[S[at]].snd, S[at]));
			at++;
		}
		if (mx.snd == -1) return {};
		cur = mx.fst;
		R.pb(mx.snd);
	}
	return R;
}
