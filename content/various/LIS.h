/**
 * Author: Johan Sannemo
 * License: CC0
 * Description: Compute indices for the longest increasing subsequence.
 * Time: $O(N \log N)$
 * Status: Tested on kattis:longincsubseq, stress-tested
 */
#pragma once

template<class I> vi lis(const vector<I>& S) {
	if (S.empty()) return {};
	vi prev(SZ(S));
	typedef pair<I, ll> p;
	vector<p> res;
	fore(i,0,SZ(S)) {
		// change 0 -> i for longest non-decreasing subsequence
		auto it = lower_bound(ALL(res), p{S[i], 0});
		if (it == res.end()) res.pb({}), it = res.end()-1;
		*it = {S[i], i};
		prev[i] = it == res.begin() ? 0 : (it-1)->snd;
	}
	ll L = SZ(res), cur = res.back().snd;
	vi ans(L);
	while (L--) ans[L] = cur, cur = prev[cur];
	return ans;
}
