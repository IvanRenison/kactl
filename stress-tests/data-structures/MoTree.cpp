#include "../utilities/template.h"

#include "../../content/various/Randin.h"

/// content/data-structures/MoTree.h
/// START diff
ll B = 447; // ~N/sqrt(Q)
/// END diff
struct MoTree {
	typedef ll T;
	/// START diff
	vi &vals;
	ll sum = 0;
	deque<ll> path;
	void add(ll pos, ll end) {
		sum += vals[pos];
		if (end == 0) path.push_front(pos);
		else path.pb(pos);
	}
	void del(ll pos, ll end) {
		sum -= vals[pos];
		assert(!path.empty());
		if (end == 0) {
			assert(path.front() == pos);
			path.pop_front();
		} else {
			assert(path.back() == pos);
			path.pop_back();
		}
	}
	T calc() { return sum; }
	/// END diff
	vector<T> solve(vector<ii> Q, vector<vi>& g, ll root=0) {
		ll N = SZ(g), pos[2] = {};
		vi s(SZ(Q)), I(N), L(N), R(N), in(N), par(N);
		vector<T> res(SZ(Q));
		add(0, 0), in[0] = 1;
		auto dfs = [&](ll x, ll p, ll dep, auto&& f) -> void {
			par[x] = p, L[x] = dep ? I[x] = N++ : N;
			for (ll y : g[x]) if (y != p) f(y, x, !dep, f);
			R[x] = !dep ? I[x] = ++N : N;
		};
		dfs(root, -1, 0, dfs);
		#define K(x) ii(I[x.fst]/B, I[x.snd] ^ -(I[x.fst]/B & 1))
		iota(ALL(s), 0);
		sort(ALL(s), [&](ll s, ll t) {return K(Q[s]) < K(Q[t]);});
		for (ll qi : s) fore(e,0,2) {
			ll &a = pos[e], b = e ? Q[qi].fst : Q[qi].snd, i = 0;
#define step(c)in[c]?(del(a,e),in[a]=0):(add(c,e),in[c]=1),a=c;
			while(!(L[b] <= L[a] && R[a] <= R[b])) I[i++]=b,b=par[b];
			while (a != b) step(par[a]);
			while (i--) step(I[i]);
			if (e) res[qi] = calc();
		}
		return res;
	}
};
/// END content

void testTr(ll n, ll q) {
	B = max((ll)(n / sqrt(max(q, 1ll))), 1ll);
	vector<ii> queries(q);
	for (auto &[l, r] : queries) {
		l = randin(0ll, n);
		r = randin(0ll, n);
	}
	vi par(n), val(n);
	fore(i,1,n) par[i] = randin(0ll, i);
	fore(i,0,n) val[i] = randin(0ll, 1000ll);
	vector<vi> g(n);
	fore(i,1,n) g[par[i]].pb(i), g[i].pb(par[i]);
	MoTree moTree({val});
	vi res = moTree.solve(queries, g);
	vi seen(n);
	fore(i,0,q) {
		// Tree depth is logarithmic, so compute query answers naively
		ll l = queries[i].fst, r = queries[i].snd;
		ll at = l;
		while (at != 0) seen[at] = 1, at = par[at];
		seen[at] = 1;
		ll sum = 0;
		while (!seen[r]) sum += val[r], r = par[r];
		at = l;
		while (at != 0) seen[at] = 0, at = par[at];
		seen[at] = 0;
		while (l != r) sum += val[l], l = par[l];
		sum += val[l];
		assert(res[i] == sum);
	}
}

int main() {
	fore(it,0,10) fore(n,1,15) fore(q,0,n*n) {
		testTr(n, q);
	}
	testTr(100'000, 100'000);
	testTr(1000, 100'000);
	testTr(100'000, 1000);
	cout << "Tests passed!" << endl;
}
