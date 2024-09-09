#include "../utilities/template.h"

#include "../../content/various/Randin.h"


void bench(ll n) {
	ll x = 0;

	// get time
	auto start = chrono::high_resolution_clock::now();
	fore(i,0,n) {
		x ^= randin(0LL, 1LL << 60);
	}
	auto end = chrono::high_resolution_clock::now();
	auto randin_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

	ll y = 0;
	start = chrono::high_resolution_clock::now();
	fore(i,0,n) {
		y ^= rand() % (1LL << 60);
	}
	end = chrono::high_resolution_clock::now();

	ll rand_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

	cout << "randin: " << randin_time << "ms" << endl;
	cout << "rand: " << rand_time << "ms" << endl;

	assert(randin_time < rand_time);
}


int main() {
	// bench(1e6); // Not running because some times speed test fails
	bench(1e8);
	// bench(1e10);
}
