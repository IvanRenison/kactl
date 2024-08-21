/**
 * Author: Sergey Slotin
 * Date: 2024-08-21
 * Description: Fast modular inverse for a constant modulus.
 * Time: O(log n), $\approx$ 2.7x faster than euclid in CF.
 * Status: stress-tested
 */
#pragma once

constexpr ll mod = 1e9 + 7;
constexpr ll k = bit_width((unsigned long long)(mod - 2));
ll inv(ll a) {
	ll r = 1;
#pragma GCC unroll(k)
	fore(l, 0, k) {
		if ((mod - 2) >> l & 1) r = r * a % mod;
		a = a * a % mod;
	}
	return r;
}
