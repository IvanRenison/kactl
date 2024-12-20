/**
 * Author: 罗穗骞, chilli
 * Date: 2019-04-11
 * License: Unknown
 * Source: Suffix array - a powerful tool for dealing with strings
 * (Chinese IOI National team training paper, 2009)
 * Description: Builds suffix array for a string.
 * \texttt{sa[i]} is the starting index of the suffix which
 * is $i$'th in the sorted suffix array.
 * The returned vector is of size $n+1$, and \texttt{sa[0] = n}.
 * The \texttt{lcp} array contains longest common prefixes for
 * neighbouring strings in the suffix array:
 * \texttt{lcp[i] = lcp(sa[i], sa[i-1])}, \texttt{lcp[0] = 0}.
 * \texttt{rank} is the inverse of the suffix array:
 * \texttt{rank[sa[i]] = i}.
 * \texttt{lim} should be strictly larger than all elements.
 * For larger alphabets use \texttt{basic\_string<ll>} instead
 * of \texttt{string}.
 * The input string must not contain any zero bytes.
 * Time: O(n \log n)
 * Status: stress-tested
 */
#pragma once

array<vi, 3> suffixArray(string& s, ll lim = 'z' + 1) {
	ll n = SZ(s) + 1, k = 0, a, b;
	vi rank(ALL(s)+1), y(n), ws(max(n,lim)), sa(n), lcp(n);
	iota(ALL(sa), 0);
	for (ll j = 0, p = 0; p < n; j = max(1ll, j * 2), lim = p) {
		p = j, iota(ALL(y), n - j), fill(ALL(ws), 0);
		fore(i,0,n) if (ws[rank[i]]++, sa[i]>=j) y[p++] = sa[i]-j;
		fore(i, 1, lim) ws[i] += ws[i - 1];
		for (ll i = n; i--;) sa[--ws[rank[y[i]]]] = y[i];
		swap(rank, y), p = 1, rank[sa[0]] = 0;
		fore(i, 1, n) a = sa[i - 1], b = sa[i], rank[b] =
			(y[a] == y[b] && y[a + j] == y[b + j]) ? p - 1 : p++;
	}
	for(ll i = 0, j; i < n - 1; lcp[rank[i++]] = k)
		for(k && k--, j = sa[rank[i] - 1]; s[i+k] == s[j+k]; k++);
	return {sa, lcp, rank};
}
