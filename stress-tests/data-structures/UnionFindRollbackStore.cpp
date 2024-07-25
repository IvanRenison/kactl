#include "../utilities/template.h"

#include "../../content/data-structures/UnionFind.h"
#include "../../content/data-structures/UnionFindRollbackStore.h"

struct Slow {
	typedef ll T; // Global data
	typedef ll D; // Component data
	T ans; // Global data value
	void merge(D& large, const D& small) {
		large = large + small;
	}

	ll n;
	vector<D> d;
	vector<ii> edges;
	UF uf;

	Slow (const vector<D>& d) : ans(SZ(d)), n(SZ(d)), d(d), uf(n) {}

	bool join(ll a, ll b) {
		edges.pb({a, b});
		bool res = uf.join(a, b);
		ans = 0;
		fore(i, 0, n) {
			if (uf.find(i) == i) {
				ans++;
			}
		}
		return res;
	}
	ll time() {
		return SZ(edges);
	}
	void rollback(ll t) {
		while (SZ(edges) > t) {
			edges.pop_back();
		}

		uf = UF(n);
		for (auto [a, b] : edges) {
			uf.join(a, b);
		}

		ans = 0;
		fore(i, 0, n) {
			if (uf.find(i) == i) {
				ans++;
			}
		}
	}
	D get(ll a) {
		D ans = 0;
		fore(i, 0, n) {
			if (uf.sameSet(a, i)) {
				merge(ans, d[i]);
			}
		}
		return ans;
	}
	ll size(ll a) {
		return uf.size(a);
	}

};


void testCase() {
	ll n = rand() % 100 + 1;
	vi d(n);
	fore(i, 0, n) {
		d[i] = rand() % 100;
	}

	RSUF uf(d);
	Slow slow(d);

	vi times_uf, times_slow;

	fore(_, 0, 1000) {
		ll ty = rand() % 5;
		if (ty == 0) { // Join
			ll a = rand() % n, b = rand() % n;
			bool res = uf.join(a, b);
			bool ress = slow.join(a, b);
			assert(res == ress);
			assert(uf.ans == slow.ans);
		} else if (ty == 1) { // Store time
			times_uf.pb(uf.time());
			times_slow.pb(slow.time());
		} else if (ty == 2) { // Rollback
			if (!times_uf.empty()) {
				ll i = rand() % SZ(times_uf);
				while (SZ(times_uf) > i + 1) {
					times_uf.pop_back();
					times_slow.pop_back();
				}

				ll t_uf = times_uf.back();
				ll t_slow = times_slow.back();

				uf.rollback(t_uf);
				slow.rollback(t_slow);

				times_uf.pop_back();
				times_slow.pop_back();

				assert(uf.ans == slow.ans);
			}
		} else if (ty == 3) { // Get value
			ll a = rand() % n;
			ll ans_uf = uf.get(a);
			ll ans_slow = slow.get(a);
			assert(ans_uf == ans_slow);
		} else { // Size
			ll a = rand() % n;
			ll ans_uf = uf.size(a);
			ll ans_slow = slow.size(a);
			assert(ans_uf == ans_slow);
		}
	}
}

int main() {
	fore(it, 0, 1000) {
		testCase();
	}
	cout << "Tests passed!" << endl;
}
