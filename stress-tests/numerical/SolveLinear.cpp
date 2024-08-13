#include "../utilities/template.h"

const ll mod = 3;
const ll nmax = 4, mmax = 4, nmmax = 10;

const ll lut[9] = {-4,-2,-3,-1,-100,1,3,2,4};

ll modinv(ll x) {
	assert(x);
	return x;
	// return lut[x+4];
}

ll solveLinear(vector<vi>& A, vi& b, vi& x) {
	ll n = SZ(A), m = SZ(x), rank = 0, br, bc;
	if (n) assert(SZ(A[0]) == m);
	vi col(m); iota(ALL(col), 0);

	fore(i,0,n) {
		ll v, bv = -1;
		fore(r,i,n) fore(c,i,m)
			if ((v = A[r][c])) {
				br = r, bc = c, bv = v;
				goto found;
			}
		fore(j,i,n) if (b[j]) return -1;
		break;
found:
		swap(A[i], A[br]);
		swap(b[i], b[br]);
		swap(col[i], col[bc]);
		fore(j,0,n) swap(A[j][i], A[j][bc]);
		bv = modinv(A[i][i]);
		fore(j,i+1,n) {
			ll fac = A[j][i] * bv % mod;
			b[j] = (b[j] - fac * b[i]) % mod;
			fore(k,i+1,m) A[j][k] = (A[j][k] - fac*A[i][k]) % mod;
		}
		rank++;
	}

	x.assign(m, 0);
	for (ll i = rank; i--;) {
		b[i] = ((b[i] * modinv(A[i][i]) % mod) + mod) % mod;
		x[col[i]] = b[i];
		fore(j,0,i)
			b[j] = (b[j] - A[j][i] * b[i]);
	}
	return rank;
}

template<class F>
void rec(ll i, ll j, vector<vi>& A, F f) {
	if (i == SZ(A)) {
		f();
	}
	else if (j == SZ(A[i])) {
		rec(i+1, 0, A, f);
	}
	else {
		fore(v,0,mod) {
			A[i][j] = v;
			rec(i, j+1, A, f);
		}
	}
}

template<class F>
void rec2(ll i, vi& A, F f) {
	if (i == SZ(A)) f();
	else {
		fore(v,0,mod) {
			A[i] = v;
			rec2(i+1, A, f);
		}
	}
}

int main() {
	fore(n,0,nmax+1) fore(m,0,mmax+1) {
		ll nm = n*m;
		if (nm > nmmax) continue;
		vector<vi> A(n, vi(m));
		vi b(n), x(m), theX(m);
		rec(0, 0, A, [&]() {
			rec2(0, b, [&]() {
				ll sols = 0;
				rec2(0, x, [&]() {
					fore(i,0,n) {
						ll v = 0;
						fore(j,0,m) v += A[i][j] * x[j];
						if (v % mod != b[i]) return;
					}
					sols++;
					if (sols == 1) theX = x;
				});
				vector<vi> A2 = A;
				vi x2 = x, b2 = b;
				ll r = solveLinear(A2, b2, x2);
				if (sols == 0) assert(r == -1);
				else if (sols == 1) assert(r == m);
				else assert(r < m);
				if (sols == 1) assert(x2 == theX);
			});
		});
	}
	cout<<"Tests passed!"<<endl;
}
