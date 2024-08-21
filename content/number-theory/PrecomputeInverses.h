/**
 * Author: Simon Lindholm
 * Date: 2016-07-24
 * License: CC0
 * Source: Russian page
 * Description: Pre-computation of modular inverses. Assumes LIM $\le$ mod and that mod is a prime.
 * Status: Works
 */
#pragma once

constexpr ll mod = 1e9+7, LIM = 2e5;
array<ll, LIM> inv;
void initInv() {
	inv[1] = 1;
	fore(i,2,LIM) inv[i] = mod - (mod/i) * inv[mod % i] % mod;
}
