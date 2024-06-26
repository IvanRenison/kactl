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
	vi indeg(SZ(gr)), ret;
	for (auto& li : gr) for (ll x : li) indeg[x]++;
	queue<ll> q; // use priority_queue for lexic. largest ans.
	fore(i,0,SZ(gr)) if (indeg[i] == 0) q.push(i);
	while (!q.empty()) {
		ll i = q.front(); // top() for priority queue
		ret.pb(i);
		q.pop();
		for (ll x : gr[i])
			if (--indeg[x] == 0) q.push(x);
	}
	return ret;
}
