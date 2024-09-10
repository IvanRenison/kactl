/**
 * Author: Pietro Palombini
 * Date: 2024-07-06
 * License: CC0
 * Description: Use two operations instead of one to make
 * rerooting linear. Usually only worth it for non-
 * $\mathcal{O}(1)$ operations. Add \texttt{merge} and
 * \texttt{extend}, and change \texttt{acc} and
 * \texttt{exclusive}. Don't use inheritance.
 *
 * \texttt{merge} should, given accumulated$(p, S)$ and
 * accumulated$(p, T)$, with $S$ and $T$ disjoint, return
 * accumulated$(p, S \cup T)$.
 *
 * \texttt{extend} should, given the answer for
 * \texttt{g[p][ei]}, return \texttt{b} such that
 * \texttt{merge(neuts[p], b, p)} = accumulated$(p,
 * \{\texttt{g[p][ei]}\})$.
 * Time: Slow O(n)
 */
#pragma once
#include "Reroot.h"

struct RerootLinear : Reroot {
	Data merge(const Data& a, const Data& b, ll p) {
		return Data{};
	}
	Data extend(const Data& a, ll p, ll ei) {
		return Data{};
	}
	void acc(Data& p_ans, const Data& child_ans, ll p, ll ei) {
		p_ans = merge(p_ans, extend(child_ans, p, ei), p);
	}
	void ex(vd& e, vd& a, Data& ne, ll v) {
		ll d = SZ(a);
		vd p(d + 1, ne), s(d + 1, ne);
		fore(i,0,d) p[i+1] = merge(p[i], a[i]=extend(a[i],v,i), v);
		for(ll i=d-1;i>=0;i--) s[i] = merge(a[i], s[i + 1], v);
		fore(i, 0, d) e[i] = merge(p[i], s[i + 1], v);
	}
};
