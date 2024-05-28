#pragma once

/*
  Prufer Code to Tree
  Complexity: O(VlogV)
 */

vector<ii> pruferCodeToTree(vector<ll> &pruferCode) {
	// Stores number count of nodes in the prufer code
	unordered_map<ll,ll> nodeCount;

	// Set of integers absent in prufer code. They are the leaves
	set<ll> leaves;

	ll len = (ll) pruferCode.size();
	ll node = len + 2;

	// Count frequency of nodes
	for ( ll i = 0; i < len; i++ ) {
		ll t = pruferCode[i];
		nodeCount[t]++;
	}

	// Find the absent nodes
	for ( ll i = 1; i <= node; i++ ) {
		if ( nodeCount.find ( i ) == nodeCount.end() ) leaves.insert ( i );
	}

	vector<ii> edges;
	/*Connect Edges*/
	for ( ll i = 0; i < len; i++ ){
		ll a = pruferCode[i]; // First node

		//Find the smallest number which is not present in prufer code now
		ll b = *leaves.begin(); // the leaf

		edges.pb({a,b}); // Edge of the tree

		leaves.erase ( b ); // Remove from absent list
		nodeCount[a]--; // Remove from prufer code
		if ( nodeCount[a] == 0 ) leaves.insert ( a ); // If a becomes absent
	}

	// The final edge
	edges.pb({*leaves.begin(), *leaves.rbegin()});
	return edges;
}

vector<ii> genRandomTree(ll n) {
	vector<ll> pruferCode;
	for (ll i=0; i<n-2; i++) {
		pruferCode.pb(rand()%(n-1) + 1);
	}
	auto edges = pruferCodeToTree(pruferCode);
	for (auto &[u, v] : edges) u--, v--;
	return edges;
}
