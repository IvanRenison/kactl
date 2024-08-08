/**
 * Author: chilli
 * Date: 2019-04-25
 * License: CC0
 * Source: http://neerc.ifmo.ru/trains/toulouse/2017/fft2.pdf
 * Description: Higher precision FFT, can be used for convolutions modulo arbitrary integers
 * as long as $N\log_2N\cdot \text{mod} < 8.6 \cdot 10^{14}$ (in practice $10^{16}$ or higher).
 * Inputs must be in $[0, \text{mod})$.
 * Time: O(N \log N), where $N = |A|+|B|$ (twice as slow as NTT or FFT)
 * Status: stress-tested
 * Details: An in-depth examination of precision for both FFT and FFTMod can be found
 * here (https://github.com/simonlindholm/fft-precision/blob/master/fft-precision.md)
 */
#pragma once

#include "FastFourierTransform.h"

template<ll M> vi convMod(const vi &a, const vi &b) {
	if (a.empty() || b.empty()) return {};
	vi res(SZ(a) + SZ(b) - 1);
	ll B=64-__builtin_clzll(SZ(res)), n=1<<B, cut=ll(sqrt(M));
	vector<C> L(n), R(n), outs(n), outl(n);
	fore(i,0,SZ(a)) L[i] = C((ll)a[i] / cut, (ll)a[i] % cut);
	fore(i,0,SZ(b)) R[i] = C((ll)b[i] / cut, (ll)b[i] % cut);
	fft(L), fft(R);
	fore(i,0,n) {
		ll j = -i & (n - 1);
		outl[j] = (L[i] + conj(L[j])) * R[i] / (2.0 * n);
		outs[j] = (L[i] - conj(L[j])) * R[i] / (2.0 * n) / 1i;
	}
	fft(outl), fft(outs);
	fore(i,0,SZ(res)) {
		ll av = ll(real(outl[i])+.5), cv = ll(imag(outs[i])+.5);
		ll bv = ll(imag(outl[i])+.5) + ll(real(outs[i])+.5);
		res[i] = ((av % M * cut + bv) % M * cut + cv) % M;
	}
	return res;
}
