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

vi topoSort(const vector<vi>& g) {
	vi d(SZ(g)), q;
	for (vi& li : g) for (ll x : li) d[x]++;
	fore(i,0,SZ(g)) if (d[i] == 0) q.pb(i);
	for(ll j=0;j<SZ(q);j++)for(ll x:g[q[j]])if(--d[x]==0)q.pb(x);
	return q;
}
