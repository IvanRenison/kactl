/**
 * Author: Simon Lindholm
 * Date: 2017-05-11
 * License: CC0
 * Source: folklore
 * Description: Computes sums a[i,j] for all i<I, j<J, and increases single elements a[i,j].
 *  Requires that the elements to be updated are known in advance (call fakeUpd() before init()).
 * Time: $O(\log^2 N)$. (Use persistent segment trees for $O(\log N)$.)
 * Status: stress-tested
 */
#pragma once

#include "FenwickTree.h"

struct FT2 {
	vector<vi> ys; vector<FT> ft;
	FT2(ll limx) : ys(limx) {}
	void fakeUpd(ll x, ll y) {
		for (; x < SZ(ys); x |= x + 1) ys[x].pb(y);
	}
	void init() {
		for (vi& v : ys) sort(ALL(v)), ft.pb(SZ(v));
	}
	ll ind(ll x, ll y) {
		return (ll)(lower_bound(ALL(ys[x]), y) - ys[x].begin()); }
	void upd(ll x, ll y, ll dif) {
		for (; x < SZ(ys); x |= x + 1)
			ft[x].upd(ind(x, y), dif);
	}
	ll query(ll x, ll y) {
		ll sum = 0;
		for (; x; x &= x - 1)
			sum += ft[x-1].query(ind(x-1, y));
		return sum;
	}
};
