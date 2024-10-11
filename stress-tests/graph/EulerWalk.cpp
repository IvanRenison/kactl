#include "../utilities/template.h"

#include "../../content/graph/EulerWalk.h"
#include "../../content/data-structures/UnionFind.h"

bool hasEulerWalk(vector<vector<ii>>& ed, ll start, bool undir, bool cycle) {
	ll n = SZ(ed);
	ll odd = 0;
	bool anyEdges = false;
	vi nins(n);
	fore(i,0,n) {
		for (auto &x : ed[i]) nins[x.fst]++;
	}
	fore(i,0,n) {
		if (!ed[i].empty()) anyEdges = true;
		if (undir) {
			assert(SZ(ed[i]) == nins[i]);
			ll nout = 0;
			for (auto &x : ed[i]) if (x.fst != i) nout++;
			if (i != start && nout % 2) odd++;
		}
		else {
			if (nins[i] == SZ(ed[i])) continue;
			if (cycle) return false;
			if (abs(nins[i] - SZ(ed[i])) > 1) { return false; }
			if (nins[i] < SZ(ed[i]) && i != start) { return false; }
		}
	}
	if (odd > !cycle) { return false; }
	if (ed[start].empty() && anyEdges) { return false; }
	UF uf(n);
	fore(i,0,n) for (auto &x : ed[i]) uf.join(i, x.fst);
	ll comp = 0;
	fore(i,0,n) if (uf.find(i) == i) {
		if (ed[i].empty()) continue;
		comp++;
	}
	return comp <= 1;
}

/// content/graph/EulerWalk.h
/// START diff
vi eulerCycle(vector<vector<ii>>& gr, ll nedges, ll src=0) {
/// END diff
	ll n = SZ(gr);
	vi D(n), its(n), eu(nedges), ret, s = {src};
	/// START diff
	// D[src]++; // to allow Euler paths, not just cycles
	/// END diff
	while (!s.empty()) {
		ll x = s.back(), &it = its[x], end = SZ(gr[x]);
		if (it == end) { ret.pb(x), s.pop_back(); continue; }
		auto [y, e] = gr[x][it++];
		if (!eu[e]) D[x]--, D[y]++, eu[e] = 1, s.pb(y);
	}
	for (ll x : D) if (x < 0 || SZ(ret) != nedges+1) return {};
	return {ret.rbegin(), ret.rend()};
}
/// END content

int main() {
	fore(cycle,0,2) fore(undir,0,2) {
		fore(it,0,10000) {
			ll n = rand() % 10 + 1;
			ll m = rand() % 20;
			ll start = rand() % n;
			vector<vector<ii>> ed(n);
			map<ii, vi> allEds;
			vector<ii> theEdges;
			fore(i,0,m) {
				ll a = rand() % n;
				ll b = rand() % n;
				ed[a].pb({b, i});
				allEds[{a, b}].pb(i);
				if (undir) {
					ed[b].pb({a, i});
					allEds[{b, a}].pb(i);
				}
				theEdges.pb({a, b});
			}

			vi res = cycle ? eulerCycle(ed, m, start) : eulerWalk(ed, m, start);
			if (0) {
				cout << n << ' ' << m << ' ' << start << ' ' << undir << ' ' << cycle << endl;
				fore(i,0,n) {
					for (auto &x : ed[i]) cout << x.fst << ' ';
					cout << endl;
				}
				cout << "returned" << endl;
				for (auto &x : res) cout << x << ' ';
				cout << endl;
				cout << "of length " << SZ(res) << endl;
			}

			if (res.empty()) {
				assert(!hasEulerWalk(ed, start, undir, cycle));
			} else {
				assert(hasEulerWalk(ed, start, undir, cycle));

				assert(res[0] == start);
				if (cycle) assert(res.back() == start);
				ll cur = start;
				vi seenEdge(m);
				fore(i,1,SZ(res)) {
					ll x = res[i];
					for (auto &eid : allEds[{cur, x}]) {
						if (!seenEdge[eid]) {
							seenEdge[eid] = 1;
							goto ok;
						}
					}
					assert(0); // no usable edge there
ok:
					cur = x;
				}
			}
		}
	}
	cout << "Tests passed!" << endl;
}
