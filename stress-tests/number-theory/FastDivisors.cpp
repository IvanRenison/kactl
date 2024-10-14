#include "../utilities/template.h"
#include "../../content/number-theory/FastDivisors.h"
#include "../../content/number-theory/Eratosthenes.h"

vi slow_divisors(ll n) {
	vi res;
	for(ll i = 1; i * i <= n; i++) {
		if(n % i == 0) {
			res.pb(i);
			if(i * i != n) res.pb(n / i);
		}
	}
	return res;
}

int main() {
	sieve();
	fore(i, 1, L) {
		vector<ii> f = fact(i);
		vi res = divisors(f);
		vi res2 = slow_divisors(i);
		sort(ALL(res));
		sort(ALL(res2));
		assert(res == res2);
	}
}
