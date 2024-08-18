#include "../utilities/template.h"

ll smallestDivisor(ll n) {
	for (ll i = 2; i * i <= n; i++) if (n % i == 0) return i;
	return n;
}

#include "../../content/number-theory/FastEratosthenes.h"
#include "../../content/number-theory/Eratosthenes.h"


int main() {
	// sieve
	assert(L == LIM);
	vi pr1 = sieve();
	vi pr2 = eratosthenes();
	assert(pr1 == pr2);
	set<ll> pr(ALL(pr1));

	assert(s[0] == 0);
	assert(s[1] == 0);
	fore(i, 2, LIM) {
		ll d = smallestDivisor(i);
		assert(s[i] == d);
		assert((d == i) == isPrime[i]);
		assert((d == i) == pr.count(i));
	}
	assert(*pr.rbegin() < LIM);

	// fact
	assert(fact(1).empty());
	fore(n, 2, LIM) {
		auto res = fact(n);
		ll cur = 1, last = 0;
		for (auto [p, e] : res) {
			// assert(isPrime[p]);
			if (!isPrime[p]) {
				cout << n << ' ' << p << endl;
				assert(isPrime[p]);
			}
			assert(e > 0);
			assert(last < p);
			last = p;
			fore(_, 0, e) cur *= p;
		}
		assert(cur == n);
		if (!is_sorted(ALL(res))) {
			cout << n << endl;
			for (auto [p, e] : res) cout << p << ' ' << e << endl;
			assert(0);
		}
	}

	cout<<"Tests passed!"<<endl;
}
