#include "../utilities/template.h"

#include "../../content/data-structures/FastRMQ.h"
namespace nlog {
	#include "../../content/data-structures/RMQ.h"
}

void testCase() {
	ll n = rand() % 1000 + 1;
	vi V(n);
	fore(i, 0, n) {
		V[i] = rand() % 1000;
	}

	RMQ<ll> rmq(V);
	nlog::RMQ<ll> rmq_nlog(V);

	fore(_, 0, 1000) {
		ll b = rand() % n + 1;
		ll a = rand() % b;
		ll res = rmq_nlog.query(a, b);
		ll res1 = rmq.query(a, b);
		assert(res == res1);
	}
}

void bench_construction() {
	for (ll n = 10; n <= 1e7; n *= 10) {
		vi V(n);
		fore(i, 0, n) {
			V[i] = rand() % 1000;
		}

		auto start = chrono::high_resolution_clock::now();
		RMQ<ll> rmq(V);
		auto end = chrono::high_resolution_clock::now();

		ll time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

		start = chrono::high_resolution_clock::now();
		nlog::RMQ<ll> rmq_nlog(V);
		end = chrono::high_resolution_clock::now();

		ll time_nlog = chrono::duration_cast<chrono::milliseconds>(end - start).count();

		cout << "n = " << n << ", On: " << time << "ms, Onlogn: " << time_nlog << "ms" << endl;
		assert(time <= time_nlog);
	}
}

void bench_queries() {
	for (ll n = 10; n <= 1e7; n *= 10) {
		vi V(n);
		fore(i, 0, n) {
			V[i] = rand() % 1000;
		}

		vector<ii> queries(1e6);
		fore(i, 0, 1e6) {
			ll b = rand() % n + 1;
			ll a = rand() % b;
			queries[i] = {a, b};
		}

		RMQ<ll> rmq(V);
		ll x0 = 0;
		auto start = chrono::high_resolution_clock::now();
		for (auto [a, b] : queries) {
			x0 ^= rmq.query(a, b);
		}
		auto end = chrono::high_resolution_clock::now();

		ll time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

		nlog::RMQ<ll> rmq_nlog(V);
		start = chrono::high_resolution_clock::now();
		for (auto [a, b] : queries) {
			x0 ^= rmq_nlog.query(a, b);
		}
		end = chrono::high_resolution_clock::now();

		assert(x0 == 0);

		ll time_nlog = chrono::duration_cast<chrono::milliseconds>(end - start).count();

		cout << "n = " << n << ", On: " << time << "ms, Onlogn: " << time_nlog << "ms" << endl;
		assert(time >= time_nlog);
	}
}


int main() {
	fore(_, 0, 1000) {
		testCase();
	}

	bench_construction();
	bench_queries();

	cout << "Tests passed!" << endl;
}
