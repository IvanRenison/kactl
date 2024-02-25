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

	pair<int, int> range(int i) const {
    if (i >= n) {
			return {i - n, i - n + 1};
		} else {
			return {range(i * 2).first, range(i * 2 + 1).second};
		}
	}

	void push(int p) {
		assert(p < 2 * n);
		s[p] = apply(s[p], l[p]);
		if (p < n) {
			l[p*2] = comb(l[p*2], l[p]);
			l[p*2+1] = comb(l[p*2+1], l[p]);
		}
		l[p] = lneut;
	}

	void pull(int p) {
		assert(p < 2 * n);
    //cerr << "pulling " << p << " (range [" << range(p).fst << ", " << range(p).snd << "))" << endl;
		if (p > 0) {
			pull(p/2);
		}
		push(p);
	}

  void pushUp(int p) {
    while (p > 0) {
      s[p] = f(s[p*2], s[p*2+1]);
      p /= 2;
    }
  }

	void update(int b, int e, L a) { // Update [b, e)
		for (b += n, e += n; b < e; b /= 2, e /= 2) {
			if (b % 2) {
				pull(b);
				l[b] = comb(l[b], a);
        push(b);
        pushUp(b/2);
				b++;
			}
			if (e % 2) {
        --e;
				pull(e);
				l[e] = comb(l[e], a);
        push(e);
        pushUp(e/2);
			}
		}
    assert(b == e);
    while (b > 0) {
      if (b < n) {
        s[b] = f(s[b*2], s[b*2+1]);
      }
      b /= 2;
    }
    s[0] = f(s[1], s[2]);
	}
	T query(int b, int e) { // query [b, e)
		T ra = tneut, rb = tneut;
		for (b += n, e += n; b < e; b /= 2, e /= 2) {
			if (b % 2) {
				pull(b);
				//cerr << "b: " << b << " s[b]: " << s[b] << " (range: [" << range(b).fst << ", " << range(b).snd << "))" << endl;
				ra = f(ra, s[b]);
        b++;
			}
			if (e % 2) {
        --e;
				pull(e);
				rb = f(s[e], rb);
				//cerr << "e: " << e << " s[e]: " << s[e] << " (range: [" << range(e).fst << ", " << range(e).snd << "))" << endl;
			}
		}
		return f(ra, rb);
	}
};
