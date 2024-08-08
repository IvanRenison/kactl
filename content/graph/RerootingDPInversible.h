/**
 * Author: Pietro Palombini
 * Date: 2024-07-06
 * License: CC0
 * Description: Make rerooting linear by defining the inverse
 * of \texttt{acc}.
 *
 * \texttt{unacc} should, given accumulated$(p, \texttt{g[p]})$ and the answer
 * for \texttt{g[p][ei]}, compute
 * accumulated$(p, \texttt{g[p]}\setminus\{\texttt{g[p][ei]}\})$ in \texttt{ans}.
 * Time: Fast O(n)
 */

void solve() {
	auto unacc = [&](Data& ans, const Data& child_ans, ll p,
					 ll ei) -> void { ans = Data{}; };
	auto exclusive=[&acc,&unacc](vd& exc,vd& a,Data& ne,ll v) {
		ll d = SZ(a);
		Data b = ne;
		fore(i, 0, d) acc(b, a[i], v, i);
		fill(begin(exc), begin(exc) + d, b);
		fore(i, 0, d) unacc(exc[i], a[i], v, i);
	};
}
