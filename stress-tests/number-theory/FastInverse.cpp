#include "../utilities/template.h"
#include "../../content/number-theory/FastInverse.h"

int main() {
	srand(2);

	fore(_, 0, 10000000) {
		ll a = rand() % (mod - 1) + 1;
		ll ainv = inv(a);
		assert((a * ainv) % mod == 1);
	}

	cout << "Tests passed!" << endl;
}
