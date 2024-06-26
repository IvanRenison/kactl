#include "../utilities/template.h"

#include "../../content/data-structures/Treap.h"

pair<Node*, Node*> split2(Node* n, ll v) {
	if (!n) return {};
	if (n->val >= v) {
		auto pa = split2(n->l, v);
		n->l = pa.snd;
		n->recalc();
		return {pa.fst, n};
	} else {
		auto pa = split2(n->r, v);
		n->r = pa.fst;
		n->recalc();
		return {n, pa.snd};
	}
}

ll ra() {
	static unsigned x;
	x *= 4176481;
	x += 193861934;
	return x >> 1;
}

int main() {
	srand(3);
	fore(it,0,1000) {
		vector<Node> nodes;
		vi exp;
		fore(i,0,10) {
			nodes.emplace_back(i*2+2);
			exp.emplace_back(i*2+2);
		}
		Node* n = 0;
		fore(i,0,10)
			n = merge(n, &nodes[i]);

		ll v = rand() % 25;
		ll left = cnt(split2(n, v).fst);
		ll rleft = (ll)(lower_bound(ALL(exp), v) - exp.begin());
		assert(left == rleft);
	}

	fore(it,0,10000) {
		vector<Node> nodes;
		vi exp;
		fore(i,0,10) nodes.emplace_back(i);
		fore(i,0,10) exp.emplace_back(i);
		Node* n = 0;
		fore(i,0,10)
			n = merge(n, &nodes[i]);

		ll i = ra() % 11, j = ra() % 11;
		if (i > j) swap(i, j);
		ll k = ra() % 11;
		if (i < k && k < j) continue;

		move(n, i, j, k);
		// cerr << i << ' ' << j << ' ' << k << endl;

		ll nk = (k >= j ? k - (j - i) : k);
		vi iv(exp.begin() + i, exp.begin() + j);
		exp.erase(exp.begin() + i, exp.begin() + j);
		exp.insert(exp.begin() + nk, ALL(iv));

		ll ind = 0;
		each(n, [&](ll x) {
			// cerr << x << ' ';
			assert(x == exp[ind++]);
		});
		// cerr << endl;
	}
	cout<<"Tests passed!"<<endl;
}
