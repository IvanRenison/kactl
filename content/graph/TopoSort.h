/**
 * Author: Unknown
 * Date: 2002-09-13
 * Source: predates tinyKACTL
 * Description: Topological sorting. Given is an oriented graph.
 * Output is an ordering of vertices, such that there are edges only from left to right.
 * If there are cycles, the returned list will have size smaller than $n$ -- nodes reachable
 * from cycles will not be returned.
 * Time: $O(|V|+|E|)$
 * Status: stress-tested
 */
#pragma once

vi topoSort(const vector<vi>& gr) {
	vi indeg(SZ(gr)), q;
	for (auto& li : gr) for (ll x : li) indeg[x]++;
	fore(i,0,SZ(gr)) if (indeg[i] == 0) q.pb(i);
	for(int j = 0; j < SZ(q); j++) for (ll x : gr[q[j]])
		if (--indeg[x] == 0) q.pb(x);
	return q;
}
