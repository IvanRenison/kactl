// Problem: https://open.kattis.com/problems/birthdayboy
// Status: Accepted
#include <bits/stdc++.h>
using namespace std;

#define fst first
#define snd second
#define pb push_back
#define fore(i, a, b) for (ll i = a, gmat = b; i < gmat; i++)
#define ALL(x) x.begin(), x.end()
#define SZ(x) (ll)(x).size()
#define mset(a, v) memset((a), (v), sizeof(a))
typedef long long ll;
typedef pair<ll, ll> ii;
typedef vector<ll> vi;

/// content/various/Dates.h
ll dateToInt(ll y, ll m, ll d) {
	return 1461*(y+4800+(m-14)/12)/4 + 367*(m-2-(m-14)/12*12)/12
		- 3*((y+4900+(m-14)/12)/100)/4 + d - 32075;
}
tuple<ll, ll, ll> intToDate(ll jd) {
	ll x = jd + 68569, n = 4*x/146097;
	x -= (146097*n + 3)/4;
	ll i = (4000*(x + 1))/1461001;
	x -= 1461*i/4 - 31;
	ll j = 80*x/2447, d = x - 2447*j/80;
	x = j/11;
	ll m = j + 2 - 12*x, y = 100*(n - 49) + i + x;
	return {y, m, d};
}
/// END content


ii solve(vector<ii>& birthdays) {
	ll n = SZ(birthdays);

	vi dates(3 * n);
	fore(i, 0, n) {
		auto [m, d] = birthdays[i];
		dates[i] = dateToInt(1, m, d);
		dates[i + n] = dateToInt(2, m, d);
		dates[i + 2 * n] = dateToInt(3, m, d);
	}

	sort(ALL(dates));

	ll oct_27 = dateToInt(1, 10, 27);

	ll best = 0;
	ll best_diff = 0;
	fore(i, 0, 2 * n - 1) {
		ll d0 = dates[i], d1 = dates[i + 1];
		if (d1 - 1 <= oct_27) {
			continue;
		}
		ll diff = d1 - 1 - d0;
		if (diff > best_diff) {
			best_diff = diff;
			best = d1 - 1;
		}
	}

	auto [y, m, d] = intToDate(best);

	return {m, d};
}

int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll n;
	cin >> n;
	vector<ii> birthdays(n);
	for (auto& [m, d] : birthdays) {
		string name;
		char c;
		cin >> name >> m >> c >> d;
		assert(c == '-');
	}

	auto [m, d] = solve(birthdays);
	if (m < 10) {
		cout << '0';
	}
	cout << m << '-';
	if (d < 10) {
		cout << '0';
	}
	cout << d << '\n';

}
