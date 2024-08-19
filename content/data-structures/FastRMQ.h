/**
 * Author: Iván Renison
 * Date: 2024-08-18
 * License: CC0
 * Source: notebook el vasito
 * Description: Like RMQ but with construction $O(|V|)$ and slower queries.
 * Time: $O(|V| + Q)$
 * Status: stress-tested
 */
#pragma once

template<class T>
struct RMQ {
	ll n;
	const T inf = 1e9; // change sign of inf for maximum
	vector<ll> mk;
	vector<T> bk, v;
	ll f(ll x) { return x >> 5; }
	RMQ(vector<T>& V) : n(SZ(V)), mk(n), bk(n, inf), v(V) {
		ll lst = 0;
		fore(i, 0, n) {
			bk[f(i)] = min(bk[f(i)], v[i]);
			while (lst && v[i-__builtin_ctzll(lst)]>v[i])// < for max
				lst ^= lst & -lst;
			mk[i] = ++lst, lst *= 2;
		}
		ll top = f(n);
		fore(k, 1, 64 - __builtin_clzll(top + 1)) {
			fore(i, 0, top - (1 << k) + 1)
				bk[top*k + i] =
					min(bk[top*(k-1) + i], bk[top*(k-1) + i + (1<<k-1)]);
		}
	}
	T get(ll st, ll en){
		return v[en-64+__builtin_clzll(mk[en-1]&((1ll<<en-st)-1))];
	}
	T query(ll s, ll e){	// [s, e)
		ll b1 = f(s), b2 = f(e - 1);
		if (b1 == b2) return get(s, e);
		T ans = min(get(s, (b1 + 1)*32), get(b2*32, e));
		s = (b1 + 1)*32;
		e = b2*32;
		if (s < e) {
			ll k = 63 - __builtin_clzll(f(e - s));
			ll top = f(n) * k;
			ans = min(ans,
				min(bk[top + f(s)], bk[top + f(e - 1) - (1<<k) + 1]));
		}
		return ans;
	}
};
