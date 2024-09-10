/**
 * Author: Pietro Palombini
 * Date: 2024-07-06
 * License: CC0
 * Description: Make rerooting linear by defining the inverse
 * of \texttt{acc}. Add \texttt{unacc} to the struct, keep
 * \texttt{acc} and \texttt{finalize}, and change \texttt{ex}.
 * Don't use inheritance.
 *
 * \texttt{unacc} should, given accumulated$(p, \texttt{g[p]})$
 * and the answer for \texttt{g[p][ei]}, compute
 * accumulated$(p,
 * \texttt{g[p]}\setminus\{\texttt{g[p][ei]}\})$ in
 * \texttt{ans}.
 * Time: Fast O(n)
 */
#pragma once
#include "Reroot.h"

struct RerootInv : Reroot {
	void unacc(Data& ans, const Data& child_ans, ll p, ll ei) {
		ans = Data{};
	}
	void ex(vd& e, vd& a, Data& ne, ll v) {
		ll d = SZ(a);
		Data b = ne;
		fore(i, 0, d) acc(b, a[i], v, i);
		fill(begin(e), begin(e) + d, b);
		fore(i, 0, d) unacc(e[i], a[i], v, i);
	}
};
