#include "../utilities/template.h"

#include "../../content/various/LIS.h"

template<class I> vi lisWeak(const vector<I>& S) {
	if (S.empty()) return {};
	vi prev(SZ(S));
	typedef pair<I, ll> p;
	vector<p> res;
	fore(i,0,SZ(S)) {
		// 0 -> i for longest non-decreasing subsequence
		auto it = lower_bound(ALL(res), p{S[i], i});
		if (it == res.end()) res.pb({}), it = res.end()-1;
		*it = {S[i], i};
		prev[i] = it == res.begin() ? 0 : (it-1)->snd;
	}
	ll L = SZ(res), cur = res.back().snd;
	vi ans(L);
	while (L--) ans[L] = cur, cur = prev[cur];
	return ans;
}

int main() {
	fore(weak,0,2) {
		auto lt = [weak](ll a, ll b) { return weak ? a <= b : a < b; };
		fore(it,0,1000000) {
			ll n = rand() % 7;
			vi v(n);
			for (auto &x : v) x = rand() % 4;
			vi inds = weak ? lisWeak(v) : lis(v);
			fore(i,0,SZ(inds)-1) {
				assert(lt(v[inds[i]], v[inds[i+1]]));
			}
			fore(bi,0,(1 << n)) {
				ll si = (ll)bitset<32>(bi).count();
				if (si <= SZ(inds)) continue;
				ll prev = LLONG_MIN;
				fore(i,0,n) if (bi & (1 << i)) {
					if (!lt(prev, v[i])) goto next;
					prev = v[i];
				}
				cout << "exists lis of size " << si << " but found only " << SZ(inds) << endl;
				for (auto &x : v) cout << x << ' ';
				cout << endl;
				abort();
	next:;
			}
		}
	}
	cout << "Tests passed!" << endl;
}
