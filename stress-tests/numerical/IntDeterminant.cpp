#include "../utilities/template.h"

const ll mod = 7; // 4

typedef vector<vi> vvi;
ll det(vvi& a) { // integer determinant
	ll n = SZ(a); ll ans = 1;
	fore(i,0,n) {
		fore(j,i+1,n) {
			while (a[j][i] != 0) { // gcd step
				ll t = a[i][i] / a[j][i];
				fore(k,i,n)
					a[i][k] = (a[i][k] - a[j][k] * t) % mod;
				swap(a[i], a[j]);
				ans *= -1;
			}
		}
		if (!a[i][i]) return 0;
		ans = ans * a[i][i] % mod;
	}
	if (ans < 0) ans += mod;
	return ans;
}

ll idet(vvi& a) { // integer determinant
	ll n = SZ(a); ll ans = 1;
	fore(i,0,n) {
		fore(j,i+1,n) {
			while (a[j][i] != 0) { // gcd step
				ll t = a[i][i] / a[j][i]; // can take mod-inv if mod p
				fore(k,i,n) a[i][k] -= a[j][k] * t;
				swap(a[i], a[j]);
				ans *= -1;
			}
		}
		if (!a[i][i]) return 0;
		ans *= a[i][i];
	}
	return ans;
}

double det(vector<vector<double>>& a) {
	ll n = SZ(a); double res = 1;
	fore(i,0,n) {
		ll b = i;
		fore(j,i+1,n) if (fabs(a[j][i]) > fabs(a[b][i])) b = j;
		if (i != b) swap(a[i], a[b]), res *= -1;
		res *= a[i][i];
		if (res == 0) return 0;
		fore(j,i+1,n) {
			double v = a[j][i] / a[i][i];
			if (v != 0) fore(k,i+1,n) a[j][k] -= v * a[i][k];
		}
	}
	return res;
}

template<class F>
void rec(ll i, ll j, vvi& A, F f) {
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
	fore(n,0,4) {
		vvi mat(n, vi(n, 0)), mat2;
		vector<vector<double>> mat3(n, vector<double>(n, 0));
		rec(0,0,mat,[&]() {
			fore(i,0,n) fore(j,0,n) mat3[i][j] = mat[i][j];
			// mat2 = mat; ll a = det(mat2);
			ll a = (ll)round(det(mat3)) % mod;
			mat2 = mat; ll b = idet(mat2) % mod;
			if (a < 0) a += mod;
			if (b < 0) b += mod;
			if (a != b) {
				fore(i,0,n) {
					fore(j,0,n) cout << mat[i][j];
					cout << endl;
				}
				cout << a << ' ' << b << endl;
				assert(a == b);
			}
		});
	}
	cout<<"Tests passed!"<<endl;
}
