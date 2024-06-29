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


void testCase() {
	vector<Node> nodes(1);
	nodes.reserve(100);
	vector<Node*> t(1);
	t[0] = &nodes[0];
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
		} else if (cnt < 99) { // Add new node
			nodes.push_back(Node());
			t.push_back(&nodes.back());
			s.push_back(Slow());
			cnt++;
		}
	}
}

namespace Extras { // Test commented code
	struct Node {
		Node *l = 0, *r = 0;
		Node *p = 0; // PARENT
		T val, acc; L lazy = lneut;
		ll y, c = 1;
		bool rev = false; // REVERSE
		Node(T val = tneut) : val(val), acc(val), y(rand()) {}
		void recalc() {
			c = 1, acc = tneut;
			if (l) l->push(), acc = f(acc, l->acc), c += l->c;
			acc = f(acc, val);
			if (r) r->push(), acc = f(acc, r->acc), c += r->c;
			if (l) l->p = this; // PARENT
			if (r) r->p = this;
		}
		void push() {
			if (rev) { // REVERSE
			  swap(l, r), rev = false;
			  if (l) l->rev ^= 1; if (r) r->rev ^= 1;
			}
			val = apply(val, lazy, 1), acc = apply(acc, lazy, c);
			if (l) l->lazy = comb(l->lazy, lazy);
			if (r) r->lazy = comb(r->lazy, lazy);
			lazy = lneut;
		}
		void pullAll() { // PARENT
		  if (p) p->pullAll();
		  push();
		}

		Node* split(ll k) {
			assert(k > 0);
			if (k >= c) return 0;
			push();
			ll cnt = l ? l->c : 0;
			if (k <= cnt) { // "k <= val" for lower_bound(k)
				Node* nl = l->split(k),* ret = l;
				l = nl;
				recalc();
				swap(*this, *ret);
				return ret;
			} else if (k == cnt + 1) { // k == val
				Node* ret = r;
				r = 0;
				recalc();
				return ret;
			} else {
				Node* ret = r->split(k - cnt - 1); // and just "k"
				recalc(), ret->recalc();
				return ret;
			}
		}
		void merge(Node* ri) {
			if (!ri) return;
			push(), ri->push();
			if (y > ri->y) {
				if (r) r->merge(ri);
				else r = ri;
			} else {
				merge(ri->l);
				ri->l = ri;
				swap(*this, *ri);
			}
			recalc();
		}
		ll pos() { // In witch position I am // PARENT
		  pullAll();
		  ll ans = l ? l->c : 0;
		  if (!p) return ans;
		  if (p->r == this) return ans + p->pos() + 1;
		  else return p->pos() + 1 - (r ? r->c : 0);
		}
		T query() { // Query full range
			push();
			return acc;
		}
		void upd(L v) { lazy = comb(lazy, v); } // Update full range
		void reverse() { rev = !rev; } // REVERSE
	};

	struct SlowNode {
		T val;
		SlowNode* prev,* next;
		SlowNode(T val = tneut) : val(val), prev(0), next(0) {}

		SlowNode* split(ll k) {
			assert(k > 0);
			SlowNode* it = this;
			for (ll i = 0; i + 1 < k; i++) it = it->next;
			SlowNode* ans = it->next;
			it->next = 0;
			if (ans) ans->prev = 0;
			return ans;
		}
		void merge(SlowNode* ri) {
			SlowNode* it = this;
			while (it->next) it = it->next;
			it->next = ri;
			if (ri) ri->prev = it;
		}
		ll pos() {
			ll ans = 0;
			for (SlowNode* it = prev; it; it = it->prev) ans++;
			return ans;
		}
		T query() {
			T ans = tneut;
			for (SlowNode* it = this; it; it = it->next) ans = f(ans, it->val);
			return ans;
		}
		void upd(L v) {
			for (SlowNode* it = this; it; it = it->next) it->val = apply(it->val, v, 1);
		}
		void reverse() {
			assert(prev == 0);
			SlowNode* it = this;
			while (it->next) it = it->next;
			SlowNode* it2 = this;
			while (it2 != it) {
				swap(it2->val, it->val);
				it2 = it2->next;
				if (it2 == it) break;
				it = it->prev;
			}
		}

		ll size() {
			ll ans = 0;
			for (SlowNode* it = this; it; it = it->next) ans++;
			return ans;
		}
	};

	void testCase() {
		ll n = rand() % 100 + 1;
		vector<Node> nodes(n);
		vector<SlowNode> slowNodes(n);

		vector<Node*> t(n);
		vector<SlowNode*> s(n);
		fore(i, 0, n) {
			t[i] = &nodes[i];
			s[i] = &slowNodes[i];
		}

		fore(_, 0, 1000) {
			ll ty = ra() % 6;
			if (ty == 0) { // Query
				ll i = ra() % t.size();

				ll ans = t[i]->query();
				ll slowAns = s[i]->query();
				assert(ans == slowAns);
			} else if (ty == 1) { // Update
				ll i = ra() % t.size();
				ll l = ra() % 100;
				t[i]->upd(l);
				s[i]->upd(l);
			} else if (ty == 2) { // Split
				ll i = ra() % t.size();
				ll cnt = s[i]->size();
				if (cnt < 2) continue;

				ll k = ra() % (cnt - 1) + 1;

				Node* new_t = t[i]->split(k);
				assert(new_t != NULL);
				SlowNode* new_s = s[i]->split(k);

				t.push_back(new_t);
				s.push_back(new_s);
			} else if (ty == 3) { // Merge
				ll i = ra() % t.size(), j = ra() % t.size();
				if (i == j) continue;
				if (i > j) swap(i, j);

				t[i]->merge(t[j]);
				s[i]->merge(s[j]);

				t.erase(t.begin() + j);
				s.erase(s.begin() + j);
			} else if (ty == 4) { // Reverse
				ll i = ra() % t.size();
				t[i]->reverse();
				s[i]->reverse();
			} else { // Get position
				ll i = ra() % t.size();
				ll ans = t[i]->pos();
				ll slowAns = s[i]->pos();
				assert(ans == slowAns);
			}
		}
	}
};

int main() {

	fore(it, 0, 1000) {
		testCase();
	}
	fore(it, 0, 500) {
		Extras::testCase();
	}

	cout << "Tests passed!" << endl;
}
