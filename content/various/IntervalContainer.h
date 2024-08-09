/**
 * Author: Simon Lindholm
 * License: CC0
 * Description: Add and remove intervals from a set of disjoint intervals.
 * Will merge the added interval with any overlapping intervals in the set when adding.
 * Intervals are [inclusive, exclusive).
 * Time: O(\log N)
 * Status: stress-tested
 */
#pragma once

set<ii>::iterator addInterval(set<ii>& is, ll L, ll R) {
	if (L == R) return is.end();
	auto it = is.lower_bound({L, R}), before = it;
	while (it != is.end() && it->fst <= R) {
		R = max(R, it->snd);
		before = it = is.erase(it);
	}
	if (it != is.begin() && (--it)->snd >= L) {
		L = min(L, it->fst);
		R = max(R, it->snd);
		is.erase(it);
	}
	return is.insert(before, {L,R});
}

void removeInterval(set<ii>& is, ll L, ll R) {
	if (L == R) return;
	auto it = addInterval(is, L, R);
	auto r2 = it->snd;
	if (it->fst == L) is.erase(it);
	else (ll&)it->snd = L;
	if (R != r2) is.emplace(R, r2);
}
