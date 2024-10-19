// Problem: https://codeforces.com/gym/104847/problem/D
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

/// content/various/DayOfWeek.h
ll DayOfWeek(ll y, ll m, ll d) {
	vi ttt = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
	y -= m < 3;
	return (y + y/4 - y/100 + y/400 + ttt[m-1] + d) % 7;
}
/// END content

const vi days_per_month = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

bool isLeap(ll y) {
	return y % 400 == 0 || (y % 100 != 0 && y % 4 == 0);
}

bool validDate(ll d, ll m, ll y) {
	if (isLeap(y) && m == 2) {
		return d <= 29;
	} else {
		return d <= days_per_month[m - 1];
	}
}

void solve(ll d0, ll m0, ll y0, ll d1, ll m1, ll y1) {
	if (!validDate(d1, m1, y1)) {
		cout << "Unspecified Server Error\n";
		return;
	}

	if (y1 > y0) {
		cout << "d:" << y1 - y0 << ' ';
	} else if (y1 < y0) {
		cout << "u:" << y0 - y1 << ' ';
	}

	if (m1 > m0) {
		cout << "r:" << m1 - m0 << ' ';
	} else if (m1 < m0) {
		cout << "l:" << m0 - m1 << ' ';
	}

	ll col = DayOfWeek(y1, m1, d1);
	ll col1 = DayOfWeek(y1, m1, 1);

	ll row = (d1 + col1 - 1) / 7;

	cout << '[' << row + 1 << "][" << col + 1 << "]\n";
}

int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll t;
	cin >> t;
	while (t--) {
		ll d0, m0, y0, d1, m1, y1;
		cin >> d0 >> m0 >> y0 >> d1 >> m1 >> y1;
		solve(d0, m0, y0, d1, m1, y1);
	}
}
