#include "../utilities/template.h"

const ll nmax = 5, mmax = 5, nmmax = 16;

typedef bitset<5> bs;

ll solveLinear(vector<bs>& A, vi& b, bs& x, ll m) {
	ll n = SZ(A), rank = 0, br;
	assert(m <= SZ(x));
	vi col(m); iota(ALL(col), 0);
	fore(i,0,n) {
		for (br=i; br<n; ++br) if (A[br].any()) break;
		if (br == n) {
			fore(j,i,n) if (b[j]) return -1;
			break;
		}
		ll bc = (ll)A[br]._Find_next(i-1);
		swap(A[i], A[br]);
		swap(b[i], b[br]);
		swap(col[i], col[bc]);
		fore(j,0,n) if (A[j][i] != A[j][bc]) {
			A[j].flip(i); A[j].flip(bc);
		}
		fore(j,i+1,n) if (A[j][i]) {
			b[j] ^= b[i];
			A[j] ^= A[i];
		}
		rank++;
	}

	x = bs();
	for (ll i = rank; i--;) {
		if (!b[i]) continue;
		x[col[i]] = 1;
		fore(j,0,i) b[j] ^= A[j][i];
	}
	return rank; // (multiple solutions if rank < m)
}

template<class F>
void rec(ll i, ll j, vector<bs>& A, ll m, F f) {
	if (i == SZ(A)) {
		f();
	}
	else if (j == m) {
		rec(i+1, 0, A, m, f);
	}
	else {
		fore(v,0,2) {
			A[i][j] = v;
			rec(i, j+1, A, m, f);
		}
	}
}

template<class F>
void rec2(ll i, bs& A, ll m, F f) {
	if (i == m) f();
	else {
		fore(v,0,2) {
			A[i] = v;
			rec2(i+1, A, m, f);
		}
	}
}

int main() {
	ll ct = 0;
	fore(n,0,nmax+1) fore(m,0,mmax+1) {
		ll nm = n*m;
		if (nm > nmmax) continue;
		vector<bs> A(n, bs(m));
		bs b, x, theX;
		vi b2(n);
		rec(0, 0, A, m, [&]() {
			rec2(0, b, n, [&]() {
				ll sols = 0;
				rec2(0, x, m, [&]() {
					fore(i,0,n) {
						ll v = 0;
						fore(j,0,m) v ^= A[i][j] & x[j];
						if (v != b[i]) return;
					}
					sols++;
					if (sols == 1) theX = x;
				});
				vector<bs> A2 = A;
				bs x2 = x; fore(i,0,n) b2[i] = b[i];
				ll r = solveLinear(A2, b2, x2, m);
				if (sols == 0) assert(r == -1);
				else if (sols == 1) assert(r == m);
				else assert(r < m);
				if (sols == 1) assert(x2 == theX);
				ct++;
			});
		});
	}
	cout << "Tests passed!" << endl;
}
