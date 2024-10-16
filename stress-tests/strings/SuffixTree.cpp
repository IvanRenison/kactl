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


int main() {

	fore(_, 0, 1000) {
		test();
		testEndSpecial();
	}

	cout << "Tests passed!" << endl;
}
