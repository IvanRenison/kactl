#include "../utilities/template.h"

#include "../../content/data-structures/IterativeLazySegmentTree.h"

namespace Slow {
struct Tree {
	typedef int T; // data type
	struct L { int mset, madd; }; // lazy type
	const T tneut = -inf;		 // neutral elements
	const L lneut = {inf, 0};
	T f (T a, T b) { return max(a, b); } // (any associative fn)
	T apply (T a, L b) {
		return b.mset != inf ? b.mset + b.madd : a + b.madd;
	} // Apply lazy
	L comb(L a, L b) {
		if (b.mset != inf) return b;
		return {a.mset, a.madd + b.madd};
	} // Combine lazy

	int n;
	vector<T> vals;

  Tree(const vector<T> vals) : n(sz(vals)), vals(vals) {}

  void update(int b, int e, L a) { // update [b, e)
    rep(i, b, e) {
      vals[i] = apply(vals[i], a);
    }
  }
  T query(int b, int e) { // query [b, e)
    T ans = tneut;
    rep(i, b, e) {
      ans = f(ans, vals[i]);
    }
    return ans;
  }
};
}

int main() {
  {
    Tree t(5);
    assert(t.query(0, 5) == t.tneut);
  }

  rep(_, 0, 1000) {
    int n = rand() % 100 + 1;

    vector<int> vals(n);
    rep(i, 0, n) {
      vals[i] = rand() % 100;
    }

    Tree t(vals);
    Slow::Tree t2(vals);

    vi as, bs, tys;

    rep(__, 0, 100) {
      int a = rand() % n, b = rand() % n, ty = rand() % 3;

      if (b == a) b++;
      if (b < a) swap(a, b);

      as.push_back(a), bs.push_back(b), tys.push_back(ty);

      if (ty <= 1) {
        int x = rand() % 100;

        if (ty == 0) {
          t.update(a, b, {x, 0});
          t2.update(a, b, {x, 0});
        } else {
          t.update(a, b, {inf, x});
          t2.update(a, b, {inf, x});
        }
      } else {
        int ans = t.query(a, b);
        int ans2 = t2.query(a, b);
        assert(ans == ans2);
      }
    }
    int a = rand() % n, b = rand() % n;
    if (b == a) b++;
    if (b < a) swap(a, b);
  }

  cout<<"Tests passed!"<<endl;
}

