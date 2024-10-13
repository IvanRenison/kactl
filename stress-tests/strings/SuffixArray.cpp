#include "../utilities/template.h"
#include "../../content/various/Randin.h"

const ll MAXA = 1e6;

namespace smallAlphabet {
#include "../../content/strings/SuffixArray.h"
}
namespace largeAlphabet {
/// content/strings/SuffixArray.h
/// START diff
array<vi, 3> suffixArray(basic_string<ll>& s, ll lim = MAXA + 1) {
	/// END diff
	ll n = SZ(s) + 1, k = 0, a, b;
	vi x(ALL(s)+1), y(n), ws(max(n,lim)), sa(n), lcp(n), rank(n);
	iota(ALL(sa), 0);
	for (ll j = 0, p = 0; p < n; j = max(1ll, j * 2), lim = p) {
		p = j, iota(ALL(y), n - j), fill(ALL(ws), 0);
		fore(i, 0, n) if (ws[x[i]]++, sa[i] >= j) y[p++] = sa[i]-j;
		fore(i, 1, lim) ws[i] += ws[i - 1];
		for (ll i = n; i--;) sa[--ws[x[y[i]]]] = y[i];
		swap(x, y), p = 1, x[sa[0]] = 0;
		fore(i, 1, n) a = sa[i - 1], b = sa[i], x[b] =
			(y[a] == y[b] && y[a + j] == y[b + j]) ? p - 1 : p++;
	}
	fore(i, 1, n) rank[sa[i]] = i;
	for(ll i = 0, j; i < n - 1; lcp[rank[i++]] = k)
		for(k && k--, j = sa[rank[i] - 1]; s[i+k] == s[j+k]; k++);
	return {sa, lcp, rank};
}
/// END content
} // namespace largeAlphabet

template <typename T> auto slow(T& s) {
	ll n = SZ(s)+1;
	vi sa(n);
	iota(ALL(sa), 0);
	sort(ALL(sa), [&](ll a, ll b) { return s.substr(a) < s.substr(b); });
	vi lcp(n);
	fore(i, 1, n) {
		ll j = 0;
		while (s[sa[i] + j] == s[sa[i - 1] + j])
			j++;
		lcp[i] = j;
	}
	vi rank(n);
	fore(i, 0, n) rank[sa[i]] = i;
	return tuple(sa, lcp, rank);
}

int main() {
	ll it = 500;
	fore(_, 0, it) {
		ll n = randin(0, 500);
		string s;
		fore(__, 0, n) s += (char)randin((ll)'a', (ll)'z' + 1);
		auto [sa, lcp, rank] = smallAlphabet::suffixArray(s);
		auto [sa2, lcp2, rank2] = slow(s);
		assert(sa == sa2);
		assert(lcp == lcp2);
		assert(rank == rank2);
	}
	fore(_, 0, it) {
		ll n = randin(0, 500);
		basic_string<ll> s;
		fore(__, 0, n) s += randin(1ll, MAXA);
		auto [sa, lcp, rank] = largeAlphabet::suffixArray(s);
		auto [sa2, lcp2, rank2] = slow(s);
		assert(sa == sa2);
		assert(lcp == lcp2);
		assert(rank == rank2);
	}
}
