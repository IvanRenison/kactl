/**
 * Author: Simon Lindholm
 * Date: 2016-12-09
 * License: CC0
 * Source: http://www.mimuw.edu.pl/~mucha/pub/mucha_sankowski_focs04.pdf
 * Description: Matching for general graphs.
 * Fails with probability $N / mod$.
 * Time: O(N^3)
 * Status: not very well tested
 */
#pragma once

#include "../numerical/MatrixInverse-mod.h"

vector<ii> generalMatching(ll N, vector<ii>& ed) {
	vector<vi> mat(N, vi(N)), A;
	for (auto [a, b] : ed) {
		ll r = rand() % mod;
		mat[a][b] = r, mat[b][a] = (mod - r) % mod;
	}

	ll r = matInv(A = mat), M = 2*N - r, fi, fj;
	assert(r % 2 == 0);

	if (M != N) do {
		mat.resize(M, vi(M));
		fore(i,0,N) {
			mat[i].resize(M);
			fore(j,N,M) {
				ll r = rand() % mod;
				mat[i][j] = r, mat[j][i] = (mod - r) % mod;
			}
		}
	} while (matInv(A = mat) != M);

	vi has(M, 1); vector<ii> ret;
	fore(it,0,M/2) {
		fore(i,0,M) if (has[i])
			fore(j,i+1,M) if (A[i][j] && mat[i][j]) {
				fi = i; fj = j; goto done;
		} assert(0); done:
		if (fj < N) ret.pb({fi, fj});
		has[fi] = has[fj] = 0;
		fore(sw,0,2) {
			ll a = modpow(A[fi][fj], mod-2);
			fore(i,0,M) if (has[i] && A[i][fj]) {
				ll b = A[i][fj] * a % mod;
				fore(j,0,M) A[i][j] = (A[i][j] - A[fi][j] * b) % mod;
			}
			swap(fi,fj);
		}
	}
	return ret;
}
