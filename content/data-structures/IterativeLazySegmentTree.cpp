const int inf = 1e9;

struct LTree {
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

	vector<T> s;
	vector<L> l;
	int n;

	LTree(int n = 0) : s(2*n, tneut), l(2*n, lneut), n(n) {}

	void upd(int p) {
		if (p < n) {
			s[p] = f(s[p*2], s[p*2+1]);
		}
	}

	void push(int p, L a) {
		assert(p < 2 * n);
		l[p] = comb(l[p], a);
		s[p] = apply(s[p], l[p]);
		if (p < n) {
			l[p*2] = comb(l[p*2], l[p]);
			l[p*2+1] = comb(l[p*2+1], l[p]);
		}
		l[p] = lneut;
	}

	void pull(int p) {
		assert(p < 2 * n);
		for (int h = __lg(p) + 1; h >= 0; h--) {
			push(p >> h, lneut);
		}
	}

	void update(int b, int e, L a) { // Update [b, e)
		pull(b + n);
    pull(e - 1 + n);
    for (b += n, e += n; b < e; b /= 2, e /= 2) {
			if (b % 2) {
        push(b, a);
				b++;
			} else {
				upd(b);
			}
			if (e % 2) {
        --e;
        push(e, a);
			} else {
				upd(e);
			}
		}
    assert(b == e);
    for (; b > 0; b /= 2) {
      upd(b);
    }
	}
	T query(int b, int e) { // query [b, e)
		T ra = tneut, rb = tneut;
		pull(b + n);
    pull(e - 1 + n);
		for (b += n, e += n; b < e; b /= 2, e /= 2) {
			if (b % 2) {
				ra = f(ra, s[b]);
        b++;
			}
			if (e % 2) {
        --e;
				rb = f(s[e], rb);
			}
		}
		return f(ra, rb);
	}
};
