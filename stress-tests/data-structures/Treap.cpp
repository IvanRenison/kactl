#include "../utilities/template.h"

#include "../../content/data-structures/Treap.h"

struct Slow {
	vector<T> vals;
	Slow(T v = tneut) : vals(vector<T>{v}) {}

	Slow slit(ll k) {
		Slow ans;
		ans.vals.clear();
		fore(i, k, SZ(vals)) ans.vals.pb(vals[i]);
		vals.resize(k);
		return ans;
	}
	void merge(Slow& o) {
		for (auto x : o.vals) vals.pb(x);
	}

	T query() {
		T ans = tneut;
		for (auto x : vals) ans = f(ans, x);
		return ans;
	}
	void upd(L l) {
		for (auto& x : vals) x = apply(x, l, 1);
	}
};

ll ra() {
	static unsigned x;
	x *= 4176481;
	x += 193861934;
	return x >> 1;
}

int main() {

	fore(it, 0, 1000) {

		vector<Node*> t(1);
		t[0] = new Node();
		vector<Slow> s(1);

		ll cnt = 0;
		fore(_, 0, 1000) {
			ll ty = ra() % 5;
			if (ty == 0) { // Query
				ll i = ra() % t.size();

				ll ans = t[i]->query();
				ll slowAns = s[i].query();
				assert(ans == slowAns);
			} else if (ty == 1) { // Update
				ll i = ra() % t.size();
				ll l = ra() % 100;
				t[i]->upd(l);
				s[i].upd(l);
			} else if (ty == 2) { // Split
				ll i = ra() % t.size();
				if (s[i].vals.size() < 2) continue;

				ll k = ra() % (s[i].vals.size() - 1) + 1;

				Node* new_t = t[i]->split(k);
				assert(new_t != NULL);
				Slow new_s = s[i].slit(k);

				t.push_back(new_t);
				s.push_back(new_s);
			} else if (ty == 3) { // Merge
				ll i = ra() % t.size(), j = ra() % t.size();
				if (i == j) continue;
				if (i > j) swap(i, j);

				t[i]->merge(t[j]);
				s[i].merge(s[j]);

				t.erase(t.begin() + j);
				s.erase(s.begin() + j);
			} else if (cnt < 100) { // Add new node
				t.push_back(new Node());
				s.push_back(Slow());
				cnt++;
			}
		}
	}

	cout << "Tests passed!" << endl;
}
