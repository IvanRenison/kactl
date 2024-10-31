/**
 * Author: Pietro Palombini
 * Date: 2024-10-30
 * License: CC0
 * Source: notebook el vasito
 * Description: Online algorithm for minimal DFA that accepts
 * the suffixes of a string \texttt{s}. All substrings of
 * \texttt{s}, and only them, are represented. Let \texttt{t}
 * the longest string represented by state
 * \texttt{v}.Then \texttt{v.len == SZ(t)}. All strings represented
 * by \texttt{v} only appear in \texttt{s} as a suffix of
 * \texttt{t}. They are the longest suffixes of \texttt{t}. The
 * rest of the suffixes of \texttt{t} are found by following
 * the suffix links \texttt{v.l}. Suffix links form the suffix
 * tree of reversed \texttt{s}. Terminal states are the ones in
 * the path from \texttt{last} to the root through suffix
 * links. Complexity is amortized: \texttt{extend} adds 1 or 2
 * states, but can change many suffix links. \texttt{last} is
 * the state representing \texttt{s}. Up to 2N states and 3N
 * transitions. For larger alphabets, use \texttt{T = ll}. For
 * performance consider \texttt{s.reserve(2*N)}, and replacing
 * \texttt{map} with \texttt{vector} or
 * \texttt{unordered\_map}.
 * Time: O(N \log K).
 * Status: stress-tested
 */

#pragma once

template <class T = char> struct SuffixAutomaton {
	struct State { ll len = 0, l = -1; map<T, ll> t; };
	vector<State> s{1}; ll last = 0;
	void extend(T c) {
		ll k = SZ(s), p = last, q;
		s.pb({}), s[k].len = s[last].len + 1;
		for(;p != -1 && !s[p].t.count(c); p = s[p].l)s[p].t[c] = k;
		if (p == -1) s[k].l = 0;
		else {
			q = s[p].t[c];
			if (s[p].len + 1 == s[q].len) s[k].l = q;
			else {
				s.pb(s[q]), s.back().len = s[p].len + 1;
				for(;p!=-1 && s[p].t[c]==q; p=s[p].l)s[p].t[c]=SZ(s)-1;
				s[q].l = s[k].l = SZ(s) - 1;
			}
		}
		last = k;
	}
};
