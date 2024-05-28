#pragma once
#include "random.h"

vector<ii> randomSimpleGraphAsEdgeList(ll n, ll m) {
	assert(m <= (ll)n * (n - 1) / 2);
	vector<ii> ed;
	if (m > (ll)n * n / 3) {
		fore(i,0,n) fore(j,0,i) {
			ll a = i, b = j;
			if (randBool()) swap(a, b);
			ed.pb({a,b});
		}
		shuffle_vec(ed);
		ed.erase(ed.begin() + m, ed.end());
	} else {
		set<ii> seen;
		fore(i,0,m) {
			ll a = randRange(n);
			ll b = randRange(n);
			if (a == b) continue;
			if (!seen.insert(minmax(a, b)).snd) continue;
			ed.pb({a,b});
		}
	}
	return ed;
}

vector<vi> randomSimpleGraph(ll n, ll m) {
	vector<vi> ed(n);
	for (auto [u, v] : randomSimpleGraphAsEdgeList(n, m)) {
		ed[u].pb(v);
		ed[v].pb(u);
	}
	for (auto& v : ed) shuffle_vec(v);
	return ed;
}

vector<ii> randomRegularGraphAsEdgeList(ll n, ll k) {
	// TODO: this is slow and requires a lot of retries for large n, change to
	// something smarter.
	assert(k < n);
	vector<ii> ed;
	vi cands(n), rem(n, k);
	fore(i,0,n) cands[i] = i;
	ll failures = 0;
	set<ii> seen;
	while (!cands.empty()) {
		if (SZ(cands) == 1) goto fail;
		ll ai = randRange(SZ(cands));
		ll bi = randRange(SZ(cands));
		ll a = cands[ai], b = cands[bi];
		if (a == b) continue;
		if (!seen.insert(minmax(a, b)).snd) {
			if (failures++ > 100) goto fail;
			continue;
		}
		failures = 0;
		ed.pb({a, b});
		--rem[a], --rem[b];
		if (ai < bi) swap(ai, bi), swap(a, b);
		if (rem[a] == 0) {
			swap(cands[ai], cands.back());
			cands.pop_back();
		}
		if (rem[b] == 0) {
			swap(cands[bi], cands.back());
			cands.pop_back();
		}
	}
	assert(SZ(ed) == n * k / 2);
	return ed;
fail:
	cerr << "retry" << endl;
	return randomRegularGraphAsEdgeList(n, k);
}
