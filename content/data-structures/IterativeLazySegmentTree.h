const int inf = 1e9;

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
	vector<T> s;
	vector<L> d;

	Tree(int n = 0) : n(n), s(2*n, tneut), d(n, lneut) {}
	Tree(const vector<T>& vals) : n(vals.size()), s(2*n), d(2*n, lneut) {
		copy(all(vals), begin(s) + n);
		for (int i = n - 1; i > 0; i--) s[i] = f(s[2*i], s[2*i+1]);
	}
	void apply_(int p, L value) {
		s[p] = apply(s[p], value);
		if (p < n) d[p] = comb(d[p], value);
	}

	void build(int p) {
		while (p > 1) p >>= 1, s[p] = apply(f(s[p<<1], s[p<<1|1]), d[p]);
	}

	void push(int p) {
		for (int s = sizeof(int) * 8 - __builtin_clz(n) + 1; s > 0; --s) {
			int i = p >> s;
			//if (d[i] != lneut) {
				apply_(i<<1, d[i]);
				apply_(i<<1|1, d[i]);
				d[i] = lneut;
			//}
		}
	}

	void update(int l, int r, L value) {
		l += n, r += n;
		push(l);
		push(r - 1);
		int l0 = l, r0 = r;
		for (; l < r; l >>= 1, r >>= 1) {
			if (l&1) apply_(l++, value);
			if (r&1) apply_(--r, value);
		}
		build(l0);
		build(r0 - 1);
	}

	int query(int l, int r) {
		l += n, r += n;
		push(l);
		push(r - 1);
		int res = -2e9;
		for (; l < r; l >>= 1, r >>= 1) {
			if (l&1) res = max(res, s[l++]);
			if (r&1) res = max(s[--r], res);
		}
		return res;
	}
};
