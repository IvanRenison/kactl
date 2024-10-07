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
	return abs(a - b) < max(1e-6, eps);
}
bool eq(const vd& a, const vd& b) {
	if (SZ(a) != SZ(b)) return false;
	fore(i,0,SZ(a)) if (!eq(a[i], b[i])) return false;
	return true;
}

chrono::nanoseconds old_time(0), new_time(0), old_max_time(0), new_max_time(0);

void testCase(vector<vd> A, vd b, vd c) {
	vd x0, x1;

	cout << "Running kactl" << endl;
	auto start = chrono::high_resolution_clock::now();
	double val1 = old::LPSolver(A, b, c).solve(x1);
	auto end = chrono::high_resolution_clock::now();
	auto this_time = chrono::duration_cast<chrono::nanoseconds>(end - start);
	old_time += this_time;
	old_max_time = max(old_max_time, this_time);
	cout << "Kactl time: " << this_time.count() << "ns" << endl;

	cout << "Running vasito" << endl;
	start = chrono::high_resolution_clock::now();
	double val0 = LPSolver(A, b, c).solve(x0);
	end = chrono::high_resolution_clock::now();
	this_time = chrono::duration_cast<chrono::nanoseconds>(end - start);
	new_time += this_time;
	new_max_time = max(new_max_time, this_time);
	cout << "Vasito time: " << this_time.count() << "ns" << endl;

	assert(eq(val0, val1));
	if (val0 != inf) {
		assert(eq(x0, x1));
	}
}

int main() {
	mt19937_64 gen(1);
	uniform_real_distribution<double> dis(-10, 10);

	cout << setprecision(1000);

	fore(_, 0, 1) { //1000000) {
		/* cerr << "Test " << _ << endl;
		ll n = rand() % 6 + 1;
		ll m = rand() % 6 + 1;
		vector<vd> A(m, vd(n));
		vd b(m), c(n);
		fore(i, 0, m) fore(j, 0, n) A[i][j] = (double)(rand() % 100000) / 10000.0 - 5.0;// dis(gen);
		fore(i, 0, m) b[i] = (double)(rand() % 100000) / 10000.0 - 5.0;// dis(gen);
		fore(i, 0, n) c[i] = (double)(rand() % 100000) / 10000.0 - 5.0;// dis(gen); */

/*
		ll n = 10;
		ll m = 2;
		vector<vd> A = {
			{7.79999999999999982236431605997495353221893310546875, 8.199999999999999289457264239899814128875732421875, 9.4000000000000003552713678800500929355621337890625, 6.20000000000000017763568394002504646778106689453125, 5.5, 0, 4.79999999999999982236431605997495353221893310546875, 3.100000000000000088817841970012523233890533447265625, 6.5999999999999996447286321199499070644378662109375, 9.9000000000000003552713678800500929355621337890625},
			{5.29999999999999982236431605997495353221893310546875, 5.4000000000000003552713678800500929355621337890625, 8.300000000000000710542735760100185871124267578125, 0.8000000000000000444089209850062616169452667236328125, 3.600000000000000088817841970012523233890533447265625, 4.79999999999999982236431605997495353221893310546875, 5.5999999999999996447286321199499070644378662109375, 5.20000000000000017763568394002504646778106689453125, 5.20000000000000017763568394002504646778106689453125, 1}
		};
		vd b = {
			9.9000000000000003552713678800500929355621337890625, 1.6999999999999999555910790149937383830547332763671875
		};
		vd c = {
			9.9000000000000003552713678800500929355621337890625, 0.299999999999999988897769753748434595763683319091796875, 6.5, 7.4000000000000003552713678800500929355621337890625, 8.699999999999999289457264239899814128875732421875, 5.9000000000000003552713678800500929355621337890625, 0.6999999999999999555910790149937383830547332763671875, 9.5999999999999996447286321199499070644378662109375, 5, 3.70000000000000017763568394002504646778106689453125
		}; */

		/* ll n = 6;
		ll m = 4;
		vector<vd> A = {
			{0.3423999999999995935695551452226936817169189453125, 3.154199999999999448618837050162255764007568359375, 4.980199999999999960209606797434389591217041015625, -1.9900999999999999801048033987171947956085205078125, 1.77420000000000044337866711430251598358154296875, -4.839999999999999857891452847979962825775146484375},
			{1.4070999999999997953636921010911464691162109375, -0.7051999999999996049382389173842966556549072265625, -0.00279999999999969162445268011651933193206787109375, -2.177799999999999958077978590154089033603668212890625, -2.985300000000000064659388954169116914272308349609375, -4.71159999999999978825826474349014461040496826171875},
			{4.5754000000000001335820343228988349437713623046875, 0.27479999999999993320898283855058252811431884765625, 0.8429000000000002046363078989088535308837890625, 0.6195000000000003836930773104541003704071044921875, -0.746900000000000119371179607696831226348876953125, 1.65340000000000042490455598453991115093231201171875},
			{4.1938999999999992951416061259806156158447265625, -3.00659999999999971720399116748012602329254150390625, 3.423700000000000187583282240666449069976806640625, -1.155299999999999993605115378159098327159881591796875, -1.265099999999999891286961428704671561717987060546875, -2.734500000000000152766688188421539962291717529296875}
		};
		vd b = {
			3.380900000000000460431692772544920444488525390625, -3.31299999999999972288833305356092751026153564453125, 0.06379999999999963478103381930850446224212646484375, 2.9962999999999997413624441833235323429107666015625
		};
		vd c = {
			4.382999999999999118927007657475769519805908203125, 4.6425000000000000710542735760100185871124267578125, 3.5053999999999998493649400188587605953216552734375, 1.3605999999999998095745468162931501865386962890625, 4.4319000000000006167510946397669613361358642578125, -4.87919999999999998152588887023739516735076904296875
		}; */

		ll n = 6;
		ll m = 4;
		vector<vd> A = {
			{0.3424, 3.1542, 4.9802, -1.9901, 1.7742, -4.84},
			{1.4071, -0.7052, -0.0028, -2.1778, -2.9853, -4.7116},
			{4.5754, 0.2748, 0.8429, 0.6195, -0.7469, 1.6534},
			{4.1939, -3.0066, 3.4237, -1.1553, -1.2651, -2.7345}
		};
		vd b = {
			3.3809, -3.313, 0.0638, 2.9963
		};
		vd c = {
			4.383, 4.6425, 3.5054, 1.3606, 4.4319, -4.8792
		};


		cout << "A:\n";
		fore(i, 0, m) {
			fore(j, 0, n) {
				cout << A[i][j] << ' ';
			}
			cout << '\n';
		}
		cout << "b:\n";
		fore(i, 0, m) {
			cout << b[i] << ' ';
		}
		cout << "\nc:\n";
		fore(i, 0, n) {
			cout << c[i] << ' ';
		}
		cout << '\n' << endl;

		testCase(A, b, c);
	}

	cout << "New time: " << new_time.count() / 1000 << " ms" << endl;
	cout << "Old time: " << old_time.count() / 1000 << " ms" << endl;
	cout << "New max time: " << new_max_time.count() / 1000 << " ms" << endl;
	cout << "Old max time: " << old_max_time.count() / 1000 << " ms" << endl;

	cout << "Tests passed!" << endl;
}
