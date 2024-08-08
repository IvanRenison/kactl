/**
 * Author: Lucian Bicsi
 * Date: 2015-06-25
 * License: GNU Free Documentation License 1.2
 * Source: csacademy
 * Description: Transform to a basis with fast convolutions of the form
 * $\displaystyle c[z] = \sum\nolimits_{z = x \oplus y} a[x] \cdot b[y]$,
 * where $\oplus$ is one of AND, OR, XOR. The size of $a$ must be a power of two.
 * Time: O(N \log N)
 * Status: stress-tested
 */
#pragma once

void FST(vi& a, bool inv) {
	for (ll n = SZ(a), step = 1; step < n; step *= 2) {
		for (ll i = 0; i < n; i += 2 * step) fore(j,i,i+step) {
			ll &u = a[j], &v = a[j + step]; tie(u, v) =
				inv ? ii(v - u, u) : ii(v, u + v); // AND
				// inv ? ii(v, u - v) : ii(u + v, u); // OR /// include-line
				// ii(u + v, u - v);                   // XOR /// include-line
		}
	}
	// if (inv) for (ll& x : a) x /= SZ(a); // XOR only /// include-line
}
vi conv(vi a, vi b) {
	FST(a, 0); FST(b, 0);
	fore(i,0,SZ(a)) a[i] *= b[i];
	FST(a, 1); return a;
}
