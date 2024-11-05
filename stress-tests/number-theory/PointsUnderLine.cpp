#include "../utilities/template.h"
#include "../../content/number-theory/PointsUnderLine.h"

ll slow_f(ll a, ll b, ll c) {
	ll count = 0;
	for (ll x = 1; a * x + b * 0 <= c; x++) {
		for (ll y = 1; a * x + b * y <= c; y++) {
			count++;
		}
	}
	return count;
}

ll slow_g(ll a, ll b, ll c, ll X, ll Y) {
	ll count = 0;
	fore(x, 1, X + 1) {
		fore(y, 1, Y + 1) {
			if (a * x + b * y <= c) count++;
		}
	}
	return count;
}

void test_f() {
	ll a = rand() % 20 + 1, b = rand() % 20 + 1;
	ll c = rand() % 21 - 10;

	ll res = f(a, b, c);
	ll expected = slow_f(a, b, c);

	assert(res == expected);
}

void test_g() {
	ll a = rand() % 100 + 1, b = rand() % 100 + 1;
	ll c = rand() % 201 - 100;
	ll X = rand() % 100 + 1, Y = rand() % 100 + 1;

	ll res = g(a, b, c, X, Y);
	ll expected = slow_g(a, b, c, X, Y);

	assert(res == expected);
}

int main() {

	fore(i, 0, 1000) {
		test_f();
		test_g();
	}

	cout << "Tests passed!" << endl;
}
