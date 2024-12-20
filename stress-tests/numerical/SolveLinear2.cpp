#include "../utilities/template.h"

typedef vector<double> vd;
const double eps = 1e-12;

enum { YES, NO, MULT };
ll solve_linear(vector<vd>& A, vd& b, vd& x) {
	ll n = SZ(A), m = SZ(x), br = -1, bc = -1;
	vi col(m); iota(ALL(col), 0);

	fore(i,0,n) {
		double v, bv = -1;
		fore(r,i,n) fore(c,i,m)
			if ((v = fabs(A[r][c])) > bv)
				br = r, bc = c, bv = v;
		if (bv <= eps) {
			fore(j,i,n) if (fabs(b[j]) > eps) return NO;
			if (i == m) break;
			return MULT;
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
	}
	if (n < m) return MULT;

	for (ll i = m; i--;) {
		x[col[i]] = (b[i] /= A[i][i]);
		fore(j,0,i)
			b[j] -= A[j][i] * b[i];
	}
	return YES;
}

int main() {
	const ll n = 1000;
	vector<vd> A(n, vd(n));
	fore(i,0,n) fore(j,0,n) A[i][j] = rand() * 1000.0 / RAND_MAX;
	vd x(n), b(n);
	fore(i,0,n) b[i] = rand() * 1000.0 / RAND_MAX;
	ll r = solve_linear(A, b, x);
	assert(r == 0);
	cout << "Tests passed!" << endl;
	// cout << r << endl;
	// fore(i,0,n) cout << x[i] << ' ';
	// cout << endl;
}
