#include "../utilities/template.h"

#include "../../content/number-theory/ModLog.h"

int main() {
	const ll lim = 100;
	fore(m,1,lim) {
		fore(a,0,lim) {
			vi ans(m, -1);
			ll b = a % m;
			fore(x,1,max(m,2ll)) {
				if (ans[b] == -1) ans[b] = x;
				b = b * a % m;
			}
			fore(b,0,m) {
				ll res = modLog(a, b, m);
				if (ans[b] != res) {
					cerr << "FAIL" << endl;
					cerr << "Expected log(" << a << ", " << b << ", " << m << ") = " << ans[b] << ", found " << res << endl;
					return 1;
				}
			}
		}
	}
	cout<<"Tests passed!"<<endl;
}
