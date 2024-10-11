#include "../utilities/template.h"

#include "../../content/strings/SuffixArray.h"

struct VecSuffixArray {
	vi sa, lcp;
	VecSuffixArray(vi &s, ll lim = 256) {
		ll n = SZ(s), k = 0;
		vi x(2 * n), y(2 * n), wv(n), ws(max(n, lim)), rank(n);
		sa = lcp = rank;
		fore(i,0,n) ws[x[i] = s[i]]++;
		fore(i,1,lim) ws[i] += ws[i - 1];
		for (ll i = n; i--;) sa[--ws[x[i]]] = i;
		for (ll j = 1, p = 0; p < n; j *= 2, lim = p) {
			p = 0;
			fore(i,n-j,n) y[p++] = i;
			fore(i,0,n) if (sa[i] >= j) y[p++] = sa[i] - j;
			fore(i,0,n) wv[i] = x[y[i]];
			fore(i,0,lim) ws[i] = 0;
			fore(i,0,n) ws[wv[i]]++;
			fore(i,1,lim) ws[i] += ws[i - 1];
			for (ll i = n; i--;) sa[--ws[wv[i]]] = y[i];
			swap(x, y), p = 1, x[sa[0]] = 0;
			fore(i,1,n) {
				ll a = sa[i-1], b = sa[i]; x[b] =
					y[a] == y[b] && y[a + j] == y[b + j] ? p - 1 : p++;
			}
		}
		fore(i,1,n) rank[sa[i]] = i;
		for (ll i = 0, j; i < n - 1; lcp[rank[i++]] = k)
			for (k && k--, j = sa[rank[i] - 1];
					s[i + k] == s[j + k]; k++) ;
	}
};

string display(const string& s) {
	string ret;
	for (auto &x : s) ret += (char)('a' + x - 1);
	return ret;
}

template<class F>
void gen(string& s, ll at, ll alpha, F f) {
	if (at == SZ(s)) f();
	else {
		fore(i,0,alpha) {
			s[at] = (char)(i + 1);
			gen(s, at+1, alpha, f);
		}
	}
}

void test(const string& s, ll alpha) {
	// cout << display(s) << endl;
	string copy = s;
	SuffixArray sa(copy, alpha+1);
	vi suffixes(SZ(s)+1), lcp(SZ(s)+1);
	iota(ALL(suffixes), 0);
	sort(ALL(suffixes), [&](ll a, ll b) {
		return s.substr(a) < s.substr(b);
	});

	if (suffixes != sa.sa) {
		cout << "sa fails for " << display(s) << ' ' << alpha << endl;
		assert(suffixes == sa.sa);
	}

	fore(i,0,SZ(s)) {
		ll j = 0;
		while (max(j + suffixes[i], suffixes[i+1]) < SZ(s) &&
				s[j + suffixes[i]] == s[j + suffixes[i+1]])
			j++;
		lcp[i+1] = j;
	}

	if (lcp != sa.lcp) {
		cout << "lcp fails for " << display(s) << ' ' << alpha << endl;
		assert(lcp == sa.lcp);
	}
}

const ll MAXN = 1e5;
namespace old {
typedef long long ll;
typedef pair<ll, ll> pli;
void count_sort(vector<pli> &b, ll bits) { // (optional)
	// this is just 3 times faster than stl sort for N=10^6
	ll mask = (1 << bits) - 1;
	for (ll it = 0; it < 2; it++) {
		ll move = it * bits;
		vi q(1 << bits), w((q).size() + 1);
		for (ll i = 0; i < SZ(b); i++)
			q[(b[i].fst >> move) & mask]++;
		partial_sum(q.begin(), q.end(), w.begin() + 1);
		vector<pli> res(b.size());
		for (ll i = 0; i < SZ(b); i++)
			res[w[(b[i].fst >> move) & mask]++] = b[i];
		swap(b, res);
	}
}
struct SuffixArray {
	vi a;
	string s;
	SuffixArray(const string &_s) : s(_s + '\0') {
		ll N = SZ(s);
		vector<pli> b(N);
		a.resize(N);
		for (ll i = 0; i < N; i++) {
			b[i].fst = s[i];
			b[i].snd = i;
		}

		ll q = 8;
		while ((1 << q) < N)
			q++;
		for (ll moc = 0;; moc++) {
			count_sort(b, q); // sort(ALL(b)) can be used as well
			a[b[0].snd] = 0;
			for (ll i = 1; i < N; i++)
				a[b[i].snd] = a[b[i - 1].snd] + (b[i - 1].fst != b[i].fst);

			if ((1 << moc) >= N)
				break;
			for (ll i = 0; i < N; i++) {
				b[i].fst = (ll)a[i] << q;
				if (i + (1 << moc) < N)
					b[i].fst += a[i + (1 << moc)];
				b[i].snd = i;
			}
		}
		for (ll i = 0; i < SZ(a); i++)
			a[i] = b[i].snd;
	}
	vi lcp() {
		// longest common prefixes: res[i] = lcp(a[i],
		// a[i-1])
		ll n = SZ(a), h = 0;
		vi inv(n), res(n);
		for (ll i = 0; i < n; i++)
			inv[a[i]] = i;
		for (ll i = 0; i < n; i++)
			if (inv[i] > 0) {
				ll p0 = a[inv[i] - 1];
				while (s[i + h] == s[p0 + h])
					h++;
				res[inv[i]] = h;
				if (h > 0)
					h--;
			}
		return res;
	}
};
} // namespace kactl

struct timeit {
	decltype(chrono::high_resolution_clock::now()) begin;
	const string label;
	timeit(string label = "???") : label(label) { begin = chrono::high_resolution_clock::now(); }
	~timeit() {
		auto end = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::milliseconds>(end - begin).count();
		cerr << duration << "ms elapsed [" << label << "]" << endl;
	}
};

signed compare() {
	srand(0);
	vi vS;
	string S;
	for (ll iter = 0; iter < 5; iter++) {

		for (ll i = 0; i < MAXN; i++) {
			ll t = rand() % 2;
			vS.pb(t + 1);
			S.pb((char)(t + 'a'));
		}

		// cout << S << endl;
		vector<array<ll, 2>> res;
		{
			timeit x("kactl");
			old::SuffixArray kactl(S);
			// cout << kactl.a[100] << endl;
			auto lcp = kactl.lcp();
			fore(i,0,SZ(S)+1)
				res.pb({kactl.a[i], lcp[i]});
		}
		{
			timeit x("MIT");
			SuffixArray sa(S);
			// cout << sa.sa[100] << endl;
			fore(i,0,SZ(S)+1) {
				assert((res[i] == array<ll, 2>{sa.sa[i], sa.lcp[i]}));
			}
		}
	}
	return 0;
}

void stress(bool onlySmall = false) {
	fore(large,0,2) {
		if (onlySmall && large) break;
		double work = large ? 1e7 : 5e5;
		fore(alpha,1,27) {
			fore(n,0,100) {
				if (n * n * pow(alpha, n) > work) break;
				// cout << alpha << ' ' << n << ": " << flush;
				string s(n, 'x');
				gen(s, 0, alpha, [&]() {
					test(s, alpha);
				});
			}
		}
	}
}

void perf() {
	string str;
	fore(i,0,300'000)
		str += (char)('a' + (rand() % 10));
	ll res = 0;
	fore(i,0,30) {
		SuffixArray sa(str);
		res += sa.sa[0];
	}
	cout << res << endl;
}

void perf2() {
	vi str;
	fore(i,0,300'000)
		str.pb(1 + (rand() % 100'000));
	str.pb(0);
	ll res = 0;
	fore(i,0,30) {
		VecSuffixArray sa(str, 101'000);
		res += sa.sa[0];
	}
	cout << res << endl;
}

int main() {
	// compare();
	stress(0);
	cout << "Tests passed!" << endl;
	// perf();
	// perf2();
}
