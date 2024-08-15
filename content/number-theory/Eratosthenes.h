/**
 * Author: Håkan Terelius
 * Date: 2009-08-26
 * License: CC0
 * Source: http://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
 * Description: \texttt{s[i]=0} if $i$ is 0, 1, else smallest
 * prime factor of $i$. \texttt{sieve} returns sorted primes
 * less than $L$. \texttt{fact} returns sorted {prime,
 * exponent} pairs of the factorization of $n$.
 * Status: Stress-tested
 */
#pragma once

const ll L = 1e6;
array<ll, L> s;
vi sieve() {
	vi p;
	for(ll i = 4; i < L; i += 2) s[i] = 2;
	for (ll i = 3; i * i < L; i += 2) if (!s[i])
		for (ll j=i*i; j < L; j += 2*i) if (!s[j]) s[j] = i;
	fore(i,2,L) if (!s[i]) p.pb(i), s[i] = i;
	return p;
}
vector<ii> fact(ll n) {
	vector<ii> res;
	for (; n > 1; n /= s[n]) {
		if (!SZ(res) || res.back().fst!=s[n]) res.pb({s[n],0});
		res.back().snd++;
	}
	return res;
}
