#include "../utilities/template.h"

#include "../../content/strings/Hashing.h"

#include <sys/time.h>
int main() {
	assert((H(1)*2+1-3).get() == 0);

	fore(it,0,10000) {
		ll n = rand() % 10;
		ll alpha = rand() % 10 + 1;
		string s;
		fore(i,0,n) s += (char)('a' + rand() % alpha);
		HashInterval hi(s);
		set<string> strs;
		set<ull> hashes;

		// HashInterval
		fore(i,0,n+1) fore(j,i,n+1) {
			string sub = s.substr(i, j - i);
			ull hash = hashString(sub).get();
			assert(hi.hashInterval(i, j).get() == hash);
			hashes.insert(hash);
			strs.insert(sub);
		}

		// getHashes
		fore(le,1,n+1) {
			auto ve = getHashes(s, le);
			assert(SZ(ve) == n-le+1);
			fore(i,0,n-le+1) {
				assert(ve[i].get() == hi.hashInterval(i, i + le).get());
			}
		}

		// No collisions
		assert(SZ(strs) == SZ(hashes));

		// Concat
		fore(_, 0, 10) {
			ll i = rand() % (n + 1);
			H h0 = hi.hashInterval(0, i);
			H h1 = hi.hashInterval(i, n);
			H h0pw = hi.pw[n - i];

			H h = concat(h0, h1, h0pw);
			assert(h == hi.hashInterval(0, n));
		}
	}
	cout<<"Tests passed!"<<endl;
}
