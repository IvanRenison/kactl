#include "../utilities/template.h"
#include "../../content/strings/SuffixTree.h"

// Get all strings to roots starting at src
vector<string> getPathsStrings(const SuffixTree& st, ll src) {
	string s;
	if (st.l[src] != -1) {
		s = st.a.substr(st.l[src], st.r[src] - st.l[src]);
	}

	vector<string> ans;
	fore(c, 0, 26) {
		if (st.t[src][c] != -1) {
			vector<string> child = getPathsStrings(st, st.t[src][c]);
			for (string& x : child) {
				ans.push_back(s + x);
			}
		}
	}

	if (ans.empty()) ans.push_back(s); // src is a root

	return ans;
}

vector<string> getLongestSuffixes(string& s) { // Get all suffixes that are not a prefix of another suffix
	ll n = SZ(s);

	vector<string> ans;
	fore(i, 0, n + 1) {
		string suf = s.substr(i);
		bool isPrefix = false;
		for (string& x : ans) {
			if (x.substr(0, n - i) == suf) {
				isPrefix = true;
				break;
			}
		}
		if (!isPrefix) ans.push_back(suf);
	}

	return ans;
}

void test(string& s) {
	SuffixTree st(s);

	vector<string> paths = getPathsStrings(st, 0);
	vector<string> suffixes = getLongestSuffixes(s);

	sort(paths.begin(), paths.end());
	sort(suffixes.begin(), suffixes.end());
	assert(paths == suffixes);
}

void test() {
	ll n = rand() % 100;
	string s;
	fore(i, 0, n) s += 'a' + rand() % 26;
	test(s);
}

void testEndSpecial() {
	ll n = rand() % 100 + 1;
	char end = 'a' + rand() % 26;
	string s;
	fore(i, 0, n - 1) {
		char c = 'a' + rand() % 26;
		if (c == end) c = 'a' + (c - 'a' + 1) % 26;
		s += c;
	}
	s += end;
	test(s);
}

namespace testLCS {

/// content/strings/SuffixTree.h
struct SuffixTree {
	/// START diff
	static constexpr ll ALPHA = 28; // alphabet size
	/// END diff
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
			l[m+1] = i, l[m] = l[v], r[m] = l[v] = q, p[m] = p[v];
			p[t[m][c] = m+1] = p[v] = t[p[m]][toi(a[l[m]])] = m;
			t[m][toi(a[q])] = v, v = s[p[m]], q = l[m];
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
		if (mask == 3) best = max(best, {len, r[node] - len});
		return mask;
	}
	static ii LCS(string s, string t) {
		SuffixTree st(s + (char)('z' + 1) + t + (char)('z' + 2));
		st.lcs(0, SZ(s), SZ(s) + 1 + SZ(t), 0);
		return st.best;
	}
};
/// END content


ii LCS(string& s, string& t) {
	ll n = SZ(s), m = SZ(t);
	ll l_best = 0, r_best = 0;
	fore(i, 0, n) {
		fore(j, 0, m) {
			ll k = 0;
			for (; k < min(n - i, m - j); k++) {
				if (s[i + k] != t[j + k]) break;
			}
			if (k > r_best - l_best) {

			}
		}
	}
	return {r_best - l_best, l_best};
}

bool isLCS(string& s, string& t, ii ans) {
	ll n = SZ(s), m = SZ(t);
	auto [len, l] = ans;
	assert(l + len <= n);
	string s_sub = s.substr(l, len);
	fore(j, 0, m - len + 1) {
		string t_sub = t.substr(j, len);
		if (s_sub == t_sub) return true;
	}
	return false;
}

void testLCS(string& s, string& t) { // Test longest common substring
	ii ans = LCS(s, t);
	ii ans2 = SuffixTree::LCS(s, t);
	assert(isLCS(s, t, ans2));
	assert(ans.fst == ans.snd);
}

void testLCS() {
	ll n = rand() % 100, m = rand() % 100;
	string s, t;
	fore(i, 0, n) s += 'a' + rand() % 26;
	fore(i, 0, m) t += 'a' + rand() % 26;
	testLCS(s, t);
}

} // namespace testLCS

int main() {

	fore(_, 0, 1000) {
		test();
		testEndSpecial();
		testLCS::testLCS();
	}

	cout << "Tests passed!" << endl;
}
