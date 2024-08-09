/**
 * Author: Per Austrin, Simon Lindholm
 * Date: 2004-02-08
 * License: CC0
 * Description: Solves $A * x = b$. If there are multiple solutions, an arbitrary one is returned.
 *  Returns rank, or -1 if no solutions. Data in $A$ and $b$ is lost.
 * Time: O(n^2 m)
 * Status: tested on kattis:equationsolver, and bruteforce-tested mod 3 and 5 for n,m <= 3
 */
#pragma once

typedef vector<double> vd;
const double eps = 1e-12;

ll solveLinear(vector<vd>& A, vd& b, vd& x) {
	ll n = SZ(A), m = SZ(x), rank = 0, br, bc;
	if (n) assert(SZ(A[0]) == m);
	vi col(m); iota(ALL(col), 0);

	fore(i,0,n) {
		double v, bv = 0;
		fore(r,i,n) fore(c,i,m)
			if ((v = fabs(A[r][c])) > bv)
				br = r, bc = c, bv = v;
		if (bv <= eps) {
			fore(j,i,n) if (fabs(b[j]) > eps) return -1;
			break;
		}
		swap(A[i], A[br]);
		swap(b[i], b[br]);
		swap(col[i], col[bc]);
		fore(j,0,n) swap(A[j][i], A[j][bc]);
		bv = 1/A[i][i];
		fore(j,i+1,n) {
			double fac = A[j][i] * bv;
			b[j] -= fac * b[i];
			fore(k,i+1,m) A[j][k] -= fac*A[i][k];
		}
		rank++;
	}

	x.assign(m, 0);
	for (ll i = rank; i--;) {
		b[i] /= A[i][i];
		x[col[i]] = b[i];
		fore(j,0,i) b[j] -= A[j][i] * b[i];
	}
	return rank; // (multiple solutions if rank < m)
}
