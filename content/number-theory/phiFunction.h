/**
 * Author: Håkan Terelius
 * Date: 2009-09-25
 * License: CC0
 * Source: http://en.wikipedia.org/wiki/Euler's_totient_function
 * Description: \emph{Euler's $\phi$} function is defined as $\phi(n):=\#$ of positive integers $\leq n$ that are coprime with $n$.
 * $\phi(1)=1$, $p$ prime $\Rightarrow \phi(p^k)=(p-1)p^{k-1}$, $m,n$ coprime $\Rightarrow \phi(mn)=\phi(m)\phi(n)$.
 * If $n=p_1^{k_1}p_2^{k_2} ... p_r^{k_r}$ then $\phi(n) = (p_1-1)p_1^{k_1-1}...(p_r-1)p_r^{k_r-1}$.
 * $\phi(n)=n \cdot \prod_{p|n}(1-1/p)$.
 *
 * $\sum_{d|n} \phi(d) = n$, $\sum_{1\leq k \leq n, \gcd(k,n)=1} k = n \phi(n)/2, n>1$
 *
 * \textbf{Euler's}: $a,n$ coprime $\Rightarrow a^{\phi(n)} \equiv 1 \pmod{n}, a^{m} \equiv a^{m \bmod{\phi(n)}} \pmod{n}$.
 *
 * \textbf{Generalization}: $m \geq \log_2(n) \Rightarrow x^m \equiv x^{\phi(n) + (m \bmod{\phi(n)})} \pmod{n}$.
 *
 * \textbf{Fermat's little thm}: $p$ prime $\Rightarrow \forall a : a^{p-1} \equiv 1 \pmod{p}$.
 * Status: Tested
 */
#pragma once

const ll LIM = 5000000;
array<ll, LIM> phi;

void calculatePhi() {
	fore(i,0,LIM) phi[i] = i&1 ? i : i/2;
	for (ll i = 3; i < LIM; i += 2) if (phi[i] == i)
		for (ll j = i; j < LIM; j += i) phi[j] -= phi[j] / i;
}
