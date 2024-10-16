/**
 * Author: Unknown
 * Date: 2017-05-15
 * Source: https://e-maxx.ru/algo/ukkonen
 * Description: Ukkonen's algorithm for online suffix tree construction.
 *  Each node contains indices [l, r) into the string, and a list of child nodes.
 *  Suffixes are given by traversals of this tree, joining [l, r) substrings.
 *  The root is 0 (has l = -1, r = 0), non-existent children are -1.
 *  To get a complete tree, append a dummy symbol -- otherwise it may contain
 *  an incomplete path (still useful for substring matching, though).
 * Time: $O(26N)$
 * Status: stress-tested a bit
 */
#pragma once

struct SuffixTree {
	static constexpr ll ALPHA = 26; // alphabet size
	ll toi(char c) { return c - 'a'; }
	string a;
	ll N, v = 0, q = 0, m = 2; // v = cur node, q = cur position
	vector<vi> t; // transitions
	vi r, l, p, s; // a[l[i]:r[i]] is substring on edge to i

	void ukkadd(ll i, ll c) {
		if (r[v] <= q)
			if (t[v][c] == -1) return l[t[v][c] = m] = i,
				q = r[v = s[p[m++] = v]], ukkadd(i, c);
			else q = l[v = t[v][c]];
		if (q == -1 || c == toi(a[q])) q++;
		else {
			l[m+1] = i, l[m] = l[v], r[m] = l[v] = q;
			p[m] = p[v], t[m][toi(a[q])] = v;
			p[t[m][c] = m+1] = p[v] = t[p[m]][toi(a[l[m]])] = m;
			v = s[p[m]], q = l[m];
			while (q < r[m]) v = t[v][toi(a[q])], q += r[v]-l[v];
			if (q == r[m]) s[m] = v;
			else s[m] = m + 2;
			q = r[v]-(q-r[m]), m += 2, ukkadd(i, c);
		}
	}

	SuffixTree(string a) : a(a), N(2*SZ(a)+2), t(N,vi(ALPHA,-1)){
		r = vi(N, SZ(a)), l = p = s = vi(N), t[1] = vi(ALPHA, 0);
		s[0] = 1, l[0] = l[1] = -1, r[0] = r[1] = p[0] = p[1] = 0;
		fore(i,0,SZ(a)) ukkadd(i, toi(a[i]));
	}

	// example: find longest common substring (uses ALPHA = 28)
	ii best;
	ll lcs(ll node, ll i1, ll i2, ll olen) {
		if (l[node] <= i1 && i1 < r[node]) return 1;
		if (l[node] <= i2 && i2 < r[node]) return 2;
		ll mask = 0, len = node ? olen + (r[node] - l[node]) : 0;
		fore(c,0,ALPHA) if (t[node][c] != -1)
			mask |= lcs(t[node][c], i1, i2, len);
		if (mask == 3)
			best = max(best, {len, r[node] - len});
		return mask;
	}
	static ii LCS(string s, string t) {
		SuffixTree st(s + (char)('z' + 1) + t + (char)('z' + 2));
		st.lcs(0, SZ(s), SZ(s) + 1 + SZ(t), 0);
		return st.best;
	}
};
