#include "../utilities/template.h"

#include "../../content/strings/MinRotation.h"

ll min_rotation2(string& v) {
	ll n = SZ(v);
	string w = v; w.insert(w.end(), ALL(v));
	ll j = 0;
	fore(i,1,n) {
		if (vi(w.begin() + i, w.begin() + i + n) <
			vi(w.begin() + j, w.begin() + j + n)) j = i;
	}
	return j;
}

void testPerf() {
	string s;
	fore(i,0,10000000)
		s += (char)(rand()%400000 < 2);
	cout << minRotation(s) << endl;
}

int main() {
	fore(it,0,1000000) {
		ll n = rand() % 10;
		string v;
		fore(i,0,n) v += (char)(rand() % 3);
		ll r = minRotation(v);
		ll r2 = min_rotation2(v);
		assert(r == r2);
		rotate(v.begin(), v.begin() + r, v.end());
		assert(minRotation(v) == 0);
		assert(min_rotation2(v) == 0);
	}
	cout << "Tests passed!" << endl;
}
