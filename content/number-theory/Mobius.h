/**
 * Author: Unknown
 * Date: 2024-10-13
 * Description: Computes the Mobius function $\mu(n)$ for all
 * $n < L$.
 * Time: O(L \log L)
 * Status: stress-tested
 */

#pragma once

#include "../../stress-tests/utilities/template.h"

const ll L = 1e6;
array<int8_t, L> mu;
void calculateMu() {
	mu[1] = 1;
	fore(i,1,L) if(mu[i]) for(ll j=2*i; j<L; j+=i) mu[j]-=mu[i];
}
