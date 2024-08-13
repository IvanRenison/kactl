/**
 * Author: Simon Lindholm
 * Date: 2016-12-08
 * Source: The regular matrix inverse code
 * Description: Invert matrix $A$ modulo a prime.
 * Returns rank; result is stored in $A$ unless singular (rank < n).
 * For prime powers, repeatedly set $A^{-1} = A^{-1} (2I - AA^{-1})\  (\text{mod }p^k)$ where $A^{-1}$ starts as
 * the inverse of A mod p, and k is doubled in each step.
 * Time: O(n^3)
 * Status: Slightly tested
 */
#pragma once

#include "../number-theory/ModPow.h"

ll matInv(vector<vi>& A) {
	ll n = SZ(A); vi col(n);
	vector<vi> tmp(n, vi(n));
	fore(i,0,n) tmp[i][i] = 1, col[i] = i;

	fore(i,0,n) {
		ll r = i, c = i;
		fore(j,i,n) fore(k,i,n) if (A[j][k]) {
			r = j; c = k; goto found;
		}
		return i;
found:
		A[i].swap(A[r]); tmp[i].swap(tmp[r]);
		fore(j,0,n)
			swap(A[j][i], A[j][c]), swap(tmp[j][i], tmp[j][c]);
		swap(col[i], col[c]);
		ll v = modpow(A[i][i], mod - 2);
		fore(j,i+1,n) {
			ll f = A[j][i] * v % mod;
			A[j][i] = 0;
			fore(k,i+1,n) A[j][k] = (A[j][k] - f*A[i][k]) % mod;
			fore(k,0,n) tmp[j][k] = (tmp[j][k] - f*tmp[i][k]) % mod;
		}
		fore(j,i+1,n) A[i][j] = A[i][j] * v % mod;
		fore(j,0,n) tmp[i][j] = tmp[i][j] * v % mod;
		A[i][i] = 1;
	}

	for (ll i = n-1; i > 0; --i) fore(j,0,i) {
		ll v = A[j][i];
		fore(k,0,n) tmp[j][k] = (tmp[j][k] - v*tmp[i][k]) % mod;
	}

	fore(i,0,n) fore(j,0,n)
		A[col[i]][col[j]] = tmp[i][j] % mod + (tmp[i][j] < 0)*mod;
	return n;
}
