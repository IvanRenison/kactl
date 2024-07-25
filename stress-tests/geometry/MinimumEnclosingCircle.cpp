#include "../utilities/template.h"

#include "../../content/geometry/MinimumEnclosingCircle.h"

int main() {
	srand(2);
	fore(it,0,1000000) {
		ll N = rand() % 20 + 1;
		// ll N = 4;
		vector<P> ps;
		fore(i,0,N) {
			ps.emplace_back(rand() % 21 - 10, rand() % 21 - 10);
		}

		pair<P, double> pa = mec(ps);
		P mid = pa.fst;
		double rad = pa.snd;
		double maxDist = 0;
		for(auto &p: ps) {
			maxDist = max(maxDist, (p - mid).dist());
		}

		assert(abs(maxDist - rad) < 1e-6);

		fore(it2,0,50) {
			P q2 = mid - P(0, 1e-6).rotate(it2);
			for(auto &p: ps) {
				if((p - q2).dist() > rad - 1e-7) goto fail;
			}
			assert(0);
fail:;
		}
	}
	cout << "Tests passed!" << endl;
}
