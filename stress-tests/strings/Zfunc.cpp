#include "../utilities/template.h"

#include "../../content/strings/Zfunc.h"

template <class F> void gen(string &s, ll at, ll alpha, F f) {
	if (at == SZ(s))
		f();
	else {
		fore(i, 0, alpha) {
			s[at] = (char)('a' + i);
			gen(s, at + 1, alpha, f);
		}
	}
}

void test(const string &s) {
	ll n = SZ(s);
	vi found = Z(s);
	vi expected(n, 0);
	fore(i, 1, n) { // exclude index 0 (!)
		ll j = 0;
		while (i + j < n && s[i + j] == s[j])
			j++;
		expected[i] = j;
	}
	assert(found == expected);
}

signed main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	fore(n, 0, 13) {
		string s(n, 'x');
		gen(s, 0, 3, [&]() { test(s); });
	}
	fore(n, 0, 11) {
		string s(n, 'x');
		gen(s, 0, 4, [&]() { test(s); });
	}
	cout << "Tests passed!" << endl;
}
