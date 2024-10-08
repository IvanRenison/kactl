/**
 * Author: Jakob Kogler, chilli, pajenegod
 * Date: 2020-04-12
 * License: CC0
 * Description: Prime sieve for generating all primes smaller than LIM.
 * Time: O(n \log \log n); LIM=1e9 $\approx$ 1.5s
 * Status: Stress-tested
 * Details: Despite its n log log n complexity, segmented sieve is still faster
 * than other options, including bitset sieves and linear sieves. This is
 * primarily due to its low memory usage, which reduces cache misses. This
 * implementation skips even numbers.
 *
 * Benchmark can be found here: https://ideone.com/e7TbX4
 *
 * The line `for (ll i=idx; i<S+L; idx = (i += p))` is done on purpose for performance reasons.
 * Se https://github.com/kth-competitive-programming/kactl/pull/166#discussion_r408354338
 */
#pragma once

const ll LIM = 1e6;
bitset<LIM> isPrime;
vi eratosthenes() {
	const ll S = (ll)round(sqrt(LIM)), R = LIM / 2;
	vi pr = {2}, sieve(S+1); pr.reserve(ll(LIM/log(LIM)*1.1));
	vector<ii> cp;
	for (ll i = 3; i <= S; i += 2) if (!sieve[i]) {
		cp.pb({i, i * i / 2});
		for (ll j = i * i; j <= S; j += 2 * i) sieve[j] = 1;
	}
	for (ll L = 1; L <= R; L += S) {
		array<bool, S> block{};
		for (auto &[p, idx] : cp)
			for (ll i=idx; i < S+L; idx = (i+=p)) block[i-L] = 1;
		fore(i,0,min(S, R - L))
			if (!block[i]) pr.pb((L + i) * 2 + 1);
	}
	for (ll i : pr) isPrime[i] = 1;
	return pr;
}
