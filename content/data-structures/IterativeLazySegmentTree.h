const ll inf = 1ll << 60;

struct Tree {
	typedef ll T; // data type
	struct L { ll mset, madd; }; // lazy type
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

	ll n;
	vector<T> s;
	vector<L> d;

	Tree(ll n = 0) : n(n), s(2*n, tneut), d(n, lneut) {}
	Tree(const vector<T>& vals) : n(vals.size()), s(2*n), d(n, lneut) {
		copy(all(vals), begin(s) + n);
		for (ll i = n - 1; i > 0; i--) s[i] = f(s[2*i], s[2*i+1]);
	}
	void apply_(ll p, L value) {
		s[p] = apply(s[p], value);
		if (p < n) d[p] = comb(d[p], value);
	}

	void build(ll p) {
		while (p > 1) p >>= 1, s[p] = apply(f(s[p<<1], s[p<<1|1]), d[p]);
	}

	void push(ll p) {
		for (ll s = 65 - __builtin_clzll(n); s > 0; --s) {
			ll i = p >> s;
			//if (d[i] != lneut) {
				apply_(i<<1, d[i]);
				apply_(i<<1|1, d[i]);
				d[i] = lneut;
			//}
		}
	}

	void update(ll l, ll r, L value) {
		l += n, r += n;
		push(l);
		push(r - 1);
		ll l0 = l, r0 = r;
		for (; l < r; l >>= 1, r >>= 1) {
			if (l&1) apply_(l++, value);
			if (r&1) apply_(--r, value);
		}
		build(l0);
		build(r0 - 1);
	}

	ll query(ll l, ll r) {
		l += n, r += n;
		push(l);
		push(r - 1);
		ll ans = -inf;
		for (; l < r; l >>= 1, r >>= 1) {
			if (l&1) ans = f(ans, s[l++]);
			if (r&1) ans = f(s[--r], ans);
		}
		return ans;
	}
};
