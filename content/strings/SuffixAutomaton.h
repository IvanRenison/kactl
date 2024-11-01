/**
 * Author: Pietro Palombini
 * Date: 2024-10-30
 * License: CC0
 * Source: notebook el vasito
 * Description: Online algorithm for minimal deterministic
 * finite automaton that accepts the suffixes of a string
 * \texttt{s}. Exactly all substrings of \texttt{s} are
 * represented by the states, each state representing one or
 * more substrings. Let \texttt{t} the longest string
 * represented by state \texttt{v}. Then \texttt{v.len ==
 * SZ(t)}, all strings represented by \texttt{v} only appear in
 * \texttt{s} as a suffix of \texttt{t} and they are the
 * longest suffixes of \texttt{t}. The rest of the suffixes of
 * \texttt{t} are found by following the suffix links
 * \texttt{v.l}. \texttt{p} is the state representing
 * \texttt{s} so terminal states are the ones in the path from
 * \texttt{p} to the root through suffix links. Also suffix
 * links form the suffix tree of reversed \texttt{s}.
 * Complexity is amortized: \texttt{extend} adds 1 or 2 states
 * but can change many suffix links. Up to 2N states and 3N
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
	vector<State> s{1}; ll p = 0;
	void extend(T c) {
		ll k=SZ(s), q; s.pb({s[p].len+1});
		for(;p != -1 && !s[p].t.count(c); p = s[p].l)s[p].t[c] = k;
		if (p == -1) s[k].l = 0;
		else if (s[p].len + 1 == s[q = s[p].t[c]].len) s[k].l = q;
		else {
			s.pb(s[q]), s.back().len = s[p].len + 1;
			for (;p!=-1 && s[p].t[c]==q; p=s[p].l) s[p].t[c]=SZ(s)-1;
			s[q].l = s[k].l = SZ(s) - 1;
		}
		p = k;
	}
};
