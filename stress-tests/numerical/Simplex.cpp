#include "../utilities/template.h"

#include "../../content/numerical/Simplex.h"

namespace old {
/**
 * Author: Stanford
 * Source: Stanford Notebook
 * License: MIT
 * Description: Solves a general linear maximization problem: maximize $c^T x$ subject to $Ax \le b$, $x \ge 0$.
 * Returns -inf if there is no solution, inf if there are arbitrarily good solutions, or the maximum value of $c^T x$ otherwise.
 * The input vector is set to an optimal $x$ (or in the unbounded case, an arbitrary solution fulfilling the constraints).
 * Numerical stability is not guaranteed. For better performance, define variables such that $x = 0$ is viable.
 * Usage:
 * vvd A = {{1,-1}, {-1,1}, {-1,-2}};
 * vd b = {1,1,-4}, c = {-1,-1}, x;
 * T val = LPSolver(A, b, c).solve(x);
 * Time: O(NM * \#pivots), where a pivot may be e.g. an edge relaxation. O(2^n) in the general case.
 * Status: seems to work?
 */

typedef double T; // long double, Rational, double + mod<P>...
typedef vector<T> vd;
typedef vector<vd> vvd;

const T eps = 1e-8, inf = 1/.0;
#define MP make_pair
#define ltj(X) if(s == -1 || MP(X[j],N[j]) < MP(X[s],N[s])) s=j

struct LPSolver {
	ll m, n;
	vi N, B;
	vvd D;

	LPSolver(const vvd& A, const vd& b, const vd& c) :
			m(SZ(b)), n(SZ(c)), N(n+1), B(m), D(m+2, vd(n+2)) {
		fore(i,0,m) fore(j,0,n) D[i][j] = A[i][j];
		fore(i,0,m) {B[i]=n+i; D[i][n]=-1; D[i][n+1]=b[i];}
		fore(j,0,n) { N[j] = j; D[m][j] = -c[j]; }
		N[n] = -1, D[m+1][n] = 1;
	}

	void pivot(ll r, ll s) {
		T *a = D[r].data(), inv = 1 / a[s];
		fore(i,0,m+2) if (i != r && abs(D[i][s]) > eps) {
			T *b = D[i].data(), inv2 = b[s] * inv;
			fore(j,0,n+2) b[j] -= a[j] * inv2;
			b[s] = a[s] * inv2;
		}
		fore(j,0,n+2) if (j != s) D[r][j] *= inv;
		fore(i,0,m+2) if (i != r) D[i][s] *= -inv;
		D[r][s] = inv;
		swap(B[r], N[s]);
	}

	bool simplex(ll phase) {
		ll x = m + phase - 1;
		for (;;) {
			ll s = -1;
			fore(j,0,n+1) if (N[j] != -phase) ltj(D[x]);
			if (D[x][s] >= -eps) return true;
			ll r = -1;
			fore(i,0,m) {
				if (D[i][s] <= eps) continue;
				if (r == -1 || MP(D[i][n+1] / D[i][s], B[i])
				             < MP(D[r][n+1] / D[r][s], B[r])) r = i;
			}
			if (r == -1) return false;
			pivot(r, s);
		}
	}

	T solve(vd &x) {
		ll r = 0;
		fore(i,1,m) if (D[i][n+1] < D[r][n+1]) r = i;
		if (D[r][n+1] < -eps) {
			pivot(r, n);
			if (!simplex(2) || D[m+1][n+1] < -eps) return -inf;
			fore(i,0,m) if (B[i] == -1) {
				ll s = 0;
				fore(j,1,n+1) ltj(D[i]);
				pivot(i, s);
			}
		}
		bool ok = simplex(1); x = vd(n);
		fore(i,0,m) if (B[i] < n) x[B[i]] = D[i][n+1];
		return ok ? D[m][n+1] : inf;
	}
};
} // namespace old

bool eq(double a, double b) {
	if (a == b) return true; // take inf into account
	double eps = 1e-6 * max(abs(a), abs(b));
	return abs(a - b) < eps;
}
bool eq(const vd& a, const vd& b) {
	if (SZ(a) != SZ(b)) return false;
	fore(i,0,SZ(a)) if (!eq(a[i], b[i])) return false;
	return true;
}

chrono::nanoseconds old_time(0), new_time(0), old_max_time(0), new_max_time(0);

void testCase(vector<vd> A, vd b, vd c) {
	vd x0, x1;

	auto start = chrono::high_resolution_clock::now();
	double val1 = old::LPSolver(A, b, c).solve(x1);
	auto end = chrono::high_resolution_clock::now();
	auto this_time = chrono::duration_cast<chrono::nanoseconds>(end - start);
	old_time += this_time;
	old_max_time = max(old_max_time, this_time);

	start = chrono::high_resolution_clock::now();
	double val0 = LPSolver(A, b, c).solve(x0);
	end = chrono::high_resolution_clock::now();
	this_time = chrono::duration_cast<chrono::nanoseconds>(end - start);
	new_time += this_time;
	new_max_time = max(new_max_time, this_time);

	assert(eq(val0, val1));
	if (val0 != inf) {
		assert(eq(x0, x1));
	}
}

int main() {
	mt19937_64 gen(1);
	uniform_real_distribution<double> dis(-10, 10);

	fore(_, 0, 10000) {
		ll n = rand() % 10 + 1;
		ll m = rand() % 10 + 1;
		vector<vd> A(m, vd(n));
		vd b(m), c(n);
		fore(i, 0, m) fore(j, 0, n) A[i][j] = dis(gen);
		fore(i, 0, m) b[i] = dis(gen);
		fore(i, 0, n) c[i] = dis(gen);
		testCase(A, b, c);
	}

	cout << "New time: " << new_time.count() / 1000 << " ms" << endl;
	cout << "Old time: " << old_time.count() / 1000 << " ms" << endl;
	cout << "New max time: " << new_max_time.count() / 1000 << " ms" << endl;
	cout << "Old max time: " << old_max_time.count() / 1000 << " ms" << endl;

	cout << "Tests passed!" << endl;
}
