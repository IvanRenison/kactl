#include "../utilities/template.h"

#include "../../content/strings/SuffixAutomaton.h"


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
    for (ll p = sa.last; p != -1; p = sa.s[p].l) {
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
    // Last represents the whole string
    assert(en == sa.last);
}

template <typename T>
T randin(T a, T b) {  // Random number in [a, b)
    static random_device rd;
    static mt19937_64 gen(rd());
    uniform_int_distribution<T> dis(a, b - 1);
    return dis(gen);
}

int main() {
    SuffixAutomaton sa;
    string s;
    ll n = 300;
    fore(_, 0, n) {
        char c = randin('a', char('z' + 1));
        s += c;
        sa.extend(c);
        run_tests<char>(sa, s);
    }
    SuffixAutomaton<ll> sa2;
    basic_string<ll> s2;
    fore(_, 0, n) {
        ll c = randin(0ll, (ll)1e6);
        s2 += c;
        sa2.extend(c);
        run_tests<ll>(sa2, s2);
    }

    cout << "All tests passed." << endl;
}
