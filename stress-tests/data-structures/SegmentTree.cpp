#include "../utilities/template.h"

static unsigned RA = 1231231;
ll ra() {
	RA *= 574841;
	RA += 14;
	return RA >> 1;
}

namespace maximum {

#include "../../content/data-structures/SegmentTree.h"

}

namespace nonabelian {

// https://en.wikipedia.org/wiki/Dihedral_group_of_order_6
const ll lut[6][6] = {
	{0, 1, 2, 3, 4, 5},
	{1, 0, 4, 5, 2, 3},
	{2, 5, 0, 4, 3, 1},
	{3, 4, 5, 0, 1, 2},
	{4, 3, 1, 2, 5, 0},
	{5, 2, 3, 1, 0, 4}
};

/// content/data-structures/SegmentTree.h
struct Tree {
	typedef ll T;
	/// START diff
	static constexpr T neut = 0;
	T f(T a, T b) { return lut[a][b]; }
	/// END diff
	vector<T> s; ll n;
	Tree(ll n = 0, T def = neut) : s(2*n, def), n(n) {}
	void upd(ll pos, T val) {
		for (s[pos += n] = val; pos /= 2;)
			s[pos] = f(s[pos * 2], s[pos * 2 + 1]);
	}
	T query(ll b, ll e) { // query [b, e)
		T ra = neut, rb = neut;
		for (b += n, e += n; b < e; b /= 2, e /= 2) {
			if (b % 2) ra = f(ra, s[b++]);
			if (e % 2) rb = f(s[--e], rb);
		}
		return f(ra, rb);
	}
};
/// END content

}

int main() {
	{
		maximum::Tree t(0);
		assert(t.query(0, 0) == t.neut);
	}

	if (0) {
		const ll N = 10000;
		maximum::Tree tr(N);
		ll sum = 0;
		fore(it,0,1000000) {
			tr.upd(ra() % N, ra());
			ll i = ra() % N;
			ll j = ra() % N;
			if (i > j) swap(i, j);
			ll v = tr.query(i, j+1);
			sum += v;
		}
		cout << sum << endl;
		// return 0;
	}

	fore(n,1,10) {
		maximum::Tree tr(n);
		vi v(n, maximum::Tree::neut);
		fore(it,0,1000000) {
			ll i = rand() % (n+1), j = rand() % (n+1);
			ll x = rand() % (n+2);

			ll r = rand() % 100;
			if (r < 30) {
				ll ma = tr.neut;
				fore(k,i,j) ma = max(ma, v[k]);
				assert(ma == tr.query(i,j));
			}
			else {
				i = min(i, n-1);
				tr.upd(i, x);
				v[i] = x;
			}
		}
	}

	fore(n,1,10) {
		nonabelian::Tree tr(n);
		vi v(n);
		fore(it,0,1000000) {
			ll i = rand() % (n+1), j = rand() % (n+1);
			ll x = rand() % 6;

			ll r = rand() % 100;
			if (r < 30) {
				ll ma = tr.neut;
				fore(k,i,j) ma = nonabelian::lut[ma][v[k]];
				assert(ma == tr.query(i,j));
			}
			else {
				i = min(i, n-1);
				tr.upd(i, x);
				v[i] = x;
			}
		}
	}
	cout << "Tests passed!" << endl;
}
