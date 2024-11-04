#include "../utilities/template.h"

#include "../../content/strings/SuffixAutomaton.h"
#include "../../content/various/Randin.h"


template <typename T>
void run_tests(const SuffixAutomaton<T> &sa, const basic_string_view<T> &s) {
	using S = basic_string<T>;
	using V = basic_string_view<T>;
	ll n = SZ(s), st = SZ(sa.s);
	vector<V> substrings;
	substrings.reserve(n * (n + 1) / 2 + 1);
	substrings.pb(s.substr(n, 0));
	fore(l, 0, SZ(s)) {
		fore(r, l + 1, SZ(s) + 1) {
			substrings.pb(s.substr(l, r - l));
		}
	}
	sort(ALL(substrings));
	substrings.erase(unique(ALL(substrings)), substrings.end());

	vector<char> terminal(st);
	for (ll p = sa.p; p != -1; p = sa.s[p].l) {
		terminal[p] = 1;
	}

	vector<vector<S>> represented(st);
	auto dfs = [&](ll u, S &cur, auto&& dfs) -> void {
		represented[u].pb(cur);
		for (auto &[c, v] : sa.s[u].t) {
			cur.pb(c);
			dfs(v, cur, dfs);
			cur.pop_back();
		}
	};
	S base{};
	dfs(0, base, dfs);

	vector<V> sa_substrings, sa_suffixes;
	sa_substrings.reserve(n * (n + 1) / 2 + 1);
	sa_suffixes.reserve(n + 1);
	fore(i, 0, SZ(sa.s)) for (auto &s : represented[i]) {
		sa_substrings.pb(s);
		if (terminal[i])
			sa_suffixes.pb(s);
	}
	sort(ALL(sa_substrings));
	sort(ALL(sa_suffixes));
	// All substrings and only substrings are represented
	assert(substrings == sa_substrings);

	vector<V> suffixes;
	fore(j, 0, SZ(s) + 1) { suffixes.pb(s.substr(j)); }
	sort(ALL(suffixes));
	// Suffixes and only suffixes are accepted
	assert(suffixes == sa_suffixes);

	fore(i, 0, SZ(represented))
		sort(ALL(represented[i]),
			 [](const S &a, const S &b) { return SZ(a) > SZ(b); });
	// All suffixes are in the upwards path
	fore(j, 0, SZ(represented)) {
		V longest = represented[j][0];
		assert(SZ(longest) == sa.s[j].len);
		ll curr = j;
		for (ll idx = 0, l = 0; l <= SZ(longest); l++) {
			assert(represented[curr][idx] == longest.substr(l));

			if (idx + 1 < SZ(represented[curr])) {
				idx++;
			} else {
				curr = sa.s[curr].l;
				idx = 0;
			}
		}
	}

	ll transitions = 0;
	fore(j, 0, SZ(sa.s)) { transitions += SZ(sa.s[j].t); }
	// Linear size
	assert(transitions <= 3 * SZ(s));
	assert(SZ(sa.s) <= 2 * SZ(s));

	// Suffix links form tree
	assert(sa.s[0].l == -1);
	fore(i, 1, SZ(sa.s)) {
		assert(sa.s[i].l != -1);
		assert(SZ(represented[i]));
		assert(sa.s[sa.s[i].l].len + SZ(represented[i]) == sa.s[i].len);
	}

	ll en = 0;
	for (auto c : s)
		en = sa.s[en].t.at(c);
	// p represents the whole string
	assert(en == sa.p);

	// DAG
	fore(u, 0, SZ(sa.s)) {
		for (auto &[c, v] : sa.s[u].t) {
			assert(sa.s[v].len > sa.s[u].len);
		}
	}
}

template <typename T>
void test_type(T lo, T hi) {
	// Online test
	fore(_, 0, 20) {
		T maxhi = randin<T>(lo, hi+1);
		SuffixAutomaton<T> sa;
		basic_string<T> s;
		ll n = 150;
		fore(it, 0, n) {
			T c = randin(lo, maxhi);
			s += c;
			sa.extend(c);
			run_tests<T>(sa, s);
		}
	}
	// Offline test
		fore(it, 0, 50) {
			T maxhi = randin<T>(lo, hi+1);
			ll n = randin(1ll, 300ll);
			SuffixAutomaton<T> sa;
			basic_string<T> s;
			fore(_, 0, n) {
				T c = randin(lo, maxhi);
				s += c;
				sa.extend(c);
			}
			run_tests<T>(sa, s);
		}
}

int main() {
	test_type<char>('a', 'z' + 1);
	test_type<ll>(0, 1e6);
	cout << "Tests passed!" << endl;
}
