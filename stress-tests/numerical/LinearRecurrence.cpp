#include "../utilities/template.h"

#include "../../content/numerical/LinearRecurrence.h"

template<class F>
void gen(vi& v, ll at, F f) {
	if (at == SZ(v)) f();
	else {
		fore(i,0,mod) {
			v[at] = i;
			gen(v, at+1, f);
		}
	}
}

int main() {
	fore(n,1,5) {
		vi start(n);
		vi coef(n);
		ll size = 10*n + 3;
		vi full(size);
		gen(start,0,[&]() {
			gen(coef,0,[&]() {
				for (auto &x : full) x = 0;
				fore(i,0,n) full[i] = start[i];
				fore(i,n,size) fore(j,0,n) full[i] = (full[i] + coef[j] * full[i-1 - j]) % mod;
	// fore(i,0,size) cerr << full[i] << ' '; cerr << endl;
	// fore(i,0,n) cerr << coef[i] << ' '; cerr << endl;
	// LinearRec lr(start, coef);
	// fore(i,0,size) { cerr << lr.Get(i) << ' '; } cerr << endl;
				fore(i,0,size) {
					auto v = linearRec(start, coef, i);
	// cerr << v << ' ';
					assert(v == full[i]);
				}
	// cerr << endl;
	// cerr << endl;
			});
		});
	}
	cout << "Tests passed!" << endl;
}
