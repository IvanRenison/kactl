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
	vector<L> l;

	Tree(int n = 0) : n(n), s(2*n, tneut), l(2*n, lneut) {}
	Tree(const vector<T>& vals) : n(vals.size()), s(2*n), l(2*n, lneut) {
		copy(all(vals), begin(s) + n);
		for (int i = n - 1; i > 0; i--) upd(i);
	}
	void push(int p, L a) {
		l[p] = comb(l[p], a);
		s[p] = apply(s[p], l[p]);
		if (p < n) {
			l[p*2] = comb(l[p*2], l[p]);
			l[p*2+1] = comb(l[p*2+1], l[p]);
		}
		l[p] = lneut;
	}
	void pull(int p) {
		for (int h = __lg(p) + 1; h >= 0; h--) push(p >> h, lneut);
	}
	void upd(int p) {
		if (p < n) s[p] = f(s[p*2], s[p*2+1]);
	}
	void update(int b, int e, L a) { // update [b, e)
		pull(b += n);
		pull((e += n) - 1);
		for (; b < e; b /= 2, e /= 2) {
			if (b % 2) push(b++, a);
			else upd(b);
			if (e % 2) push(--e, a);
			else upd(e);
		}
		for (; b > 0; b /= 2) upd(b);
	}
	T query(int b, int e) { // query [b, e)
		T ra = tneut, rb = tneut;
		pull(b += n);
		pull((e += n) - 1);
		for (; b < e; b /= 2, e /= 2) {
			if (b % 2) ra = f(ra, s[b++]);
			if (e % 2) rb = f(s[--e], rb);
		}
		return f(ra, rb);
	}
};
