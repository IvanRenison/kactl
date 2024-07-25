#include "../utilities/template.h"

#include "../../content/data-structures/UnionFind.h"
#include "../../content/graph/DynamicConnectivity.h"

struct Slow {
	ll n;
	set<ii> edges;
	UF uf;

	vector<T> ans;

	Slow (ll n) : n(n), uf(n) {}

	void add(ll a, ll b) {
		edges.insert({a, b});
		uf.join(a, b);
	}
	void remove(ll a, ll b) {
		edges.erase({a, b});
		uf = UF(n);
		for (auto [a, b] : edges) {
			uf.join(a, b);
		}
	}
	void query() {
		ll this_ans = 0;
		fore(i, 0, n) {
			if (uf.find(i) == i) {
				this_ans++;
			}
		}
		ans.push_back(this_ans);
	}

};

void testCase() {
	ll n = rand() % 100 + 1;

	DynCon dc(n);
	Slow slow(n);

	ll q = rand() % 1000 + 1;
	fore(_, 0, q) {
		ll ty = rand() % 3;
		if (ty == 0) { // Add edge
			ll a = rand() % n, b = rand() % n;
			if (a != b && !dc.last.count({a, b}) && !dc.last.count({b, a})) {
				dc.add(a, b);
				slow.add(a, b);
			}
		} else if (ty == 1) { // Remove edge
			if (!slow.edges.empty()) {
				ll i = rand() % SZ(slow.edges);
				auto [a, b] = *next(slow.edges.begin(), i);
				dc.remove(a, b);
				slow.remove(a, b);
			}
		} else { // Query
			dc.query();
			slow.query();
		}
	}

	dc.process();

	assert(dc.ans == slow.ans);
}

int main() {
	fore(it, 0, 1000) {
		testCase();
	}
	cout << "Tests passed!" << endl;
}
