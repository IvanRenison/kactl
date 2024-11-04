#include "../utilities/template.h"

#include "../../content/various/Randin.h"

/// content/strings/Hashing.h
// Arithmetic mod 2^64-1. 2x slower than mod 2^64 and more
// code, but works on evil test data (e.g. Thue-Morse, where
// ABBA... and BAAB... of length 2^10 hash the same mod 2^64).
// "typedef ull H;" instead if you think test data is random,
// or work mod 10^9+7 if the Birthday paradox is not a problem.
typedef uint64_t ull;
/// START diff
typedef ull H;
/// END diff
static const H C = (ll)1e11+3; // (order ~ 3e9; random also ok)

struct HashInterval {
	vector<H> ha, pw;
	/// START diff
	HashInterval(vi& str) : ha(SZ(str)+1), pw(ha) {
	/// END diff
		pw[0] = 1;
		fore(i,0,SZ(str))
			ha[i+1] = ha[i] * C + str[i],
			pw[i+1] = pw[i] * C;
	}
	H hashInterval(ll a, ll b) { // hash [a, b)
		return ha[b] - ha[a] * pw[b - a];
	}
};
/// START diff
/// END diff
/// END content

const ull invC = 18245175647506858667ULL;

namespace SQRT {
/// content/data-structures/MoQueries.h
/// START diff
ll B = 447; // ~N/sqrt(Q)
/// END diff
struct MoQueries {
	/// START diff
	typedef H T;
	vi& a;
	vector<H>& pws;
	H ans = 0;
	ll sz = 0;
	void add(ll pos, bool end) {
		if (end)
			ans = ans * C + a[pos];
		else
			ans = pws[sz] * a[pos] + ans;
		sz++;
	}
	void rem(ll pos, bool end) {
		sz--;
		if (end)
			ans = (ans - a[pos]) * invC;
		else
			ans = ans - pws[sz] * a[pos];
	}
	T calc() { return ans; }
	/// END diff
	ii k(ii &x){return ii(x.fst/B, x.snd ^ -(x.fst/B&1));}// SQ
	vector<T> solve(ll n, vector<ii> &qs) {
		ll l=0,r=0,q=SZ(qs); //,k=bit_width((unsigned)n), rx,ry;
		vi p(q); //,o(q);
		iota(ALL(p), 0);
	//fore(i, 0, q) {
	//  ll x = qs[i].fst, y = qs[i].snd;
	//  for (ll s = k; --s;) {
	//    rx=(x>>s)&1, ry=(y>>s)&1, o[i] += ((rx*3)^ry) << (2*s);
	//    if (!ry) {
	//      if (rx) x = (1 << k) - 1 - x, y = (1 << k) - 1 - y;
	//      swap(x, y);
	//    }
	//  }
	//}
	//sort(ALL(p), [&](ll i, ll j) { return o[i] < o[j]; });
		sort(ALL(p),[&](ll i,ll j){return k(qs[i])<k(qs[j]);});//SQ
		vector<T> res(q);
		for (ll i : p) {
				auto [ql, qr] = qs[i];
				while (l > ql) add(--l, 0);
				while (r < qr) add(r++, 1);
				while (l < ql) rem(l++, 0);
				while (r > qr) rem(--r, 1);
				res[i] = calc();
		}
		return res;
}
};
/// END content
}

namespace Hilbert {
/// content/data-structures/MoQueries.h
/// START diff
ll B = 447; // ~N/sqrt(Q)
/// END diff
struct MoQueries {
	/// START diff
	typedef H T;
	vi& a;
	vector<H>& pws;
	H ans = 0;
	ll sz = 0;
	void add(ll pos, bool end) {
		if (end)
			ans = ans * C + a[pos];
		else
			ans = pws[sz] * a[pos] + ans;
		sz++;
	}
	void rem(ll pos, bool end) {
		sz--;
		if (end)
			ans = (ans - a[pos]) * invC;
		else
			ans = ans - pws[sz] * a[pos];
	}
	T calc() { return ans; }
	// ii k(ii &x){return ii(x.fst/B, x.snd ^ -(x.fst/B&1));}// SQ
	/// END diff
	vector<T> solve(ll n, vector<ii> &qs) {
		/// START diff
		ll l=0,r=0,q=SZ(qs),k=bit_width((unsigned)n), rx,ry;
		vi p(q),o(q);
		/// END diff
		iota(ALL(p), 0);
		/// START diff
		fore(i, 0, q) {
			ll x = qs[i].fst, y = qs[i].snd;
			for (ll s = k; --s;) {
				rx=(x>>s)&1, ry=(y>>s)&1, o[i] += ((rx*3)^ry) << (2*s);
				if (!ry) {
					if (rx) x = (1 << k) - 1 - x, y = (1 << k) - 1 - y;
					swap(x, y);
				}
			}
		}
		sort(ALL(p), [&](ll i, ll j) { return o[i] < o[j]; });
		// sort(ALL(p),[&](ll i,ll j){return k(qs[i])<k(qs[j]);});//SQ
		/// END diff
		vector<T> res(q);
		for (ll i : p) {
				auto [ql, qr] = qs[i];
				while (l > ql) add(--l, 0);
				while (r < qr) add(r++, 1);
				while (l < ql) rem(l++, 0);
				while (r > qr) rem(--r, 1);
				res[i] = calc();
		}
		return res;
}
};
/// END content
}

void test(ll n, ll q, vector<H> &pws) {
	vi a(n);
	fore(i, 0, n) a[i] = randin(0ll, (ll)1e9);
	vector<ii> qs(q);
	fore(i, 0, q) {
		ll l = randin(0ll, n), r = randin(l, n+1);
		qs[i] = {l, r};
	}

	HashInterval hi(a);
	SQRT::MoQueries sq{a, pws};
	Hilbert::MoQueries hb{a, pws};

	vector<H> true_sol;
	for(auto& [l, r] : qs) {
		true_sol.pb(hi.hashInterval(l, r));
	}
	vector<H> sq_sol = sq.solve(SZ(a), qs), hb_sol = hb.solve(SZ(a), qs);

	assert(true_sol == sq_sol);
	assert(true_sol == hb_sol);
}

random_device rd;
mt19937_64 gen(rd());

int main() {
	ll maxn = 1e4, maxq = 1e5;
	vector<H> pws(maxn);
	pws[0] = 1;
	fore(i, 1, maxn) pws[i] = pws[i-1] * C;

	// Edge cases
	fore(_, 0, 20) fore(n, 0, 10) fore(q, 0, n*n)
		test(n, q, pws);

	// Random cases
	fore(_, 0, 200) {
		ll n = randin(1ll, maxn), q = randin(1ll, maxq);
		test(n, q, pws);
	}

	cout << "Tests passed" << endl;
}
