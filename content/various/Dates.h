/**
 * Author: Iván Renison
 * Date: 2024-08-29
 * License: CC0
 * Source: notebook el vasito
 * Description: Convert dates to numbers and vice versa.
 * Days and months start from 1. 1/1/1 is day number 1721426.
 * Time: O(1)
 * Status: problem tested
 */
#pragma once

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
