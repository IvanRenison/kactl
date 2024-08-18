/**
 * Author: Iván Renison
 * Date: 2024-08-18
 * License: CC0
 * Source: folklore
 * Description: Fast and secure integer uniform random numbers.
 * For floating point numbers use uniform\_real\_distribution.
 * Time: ~2x faster than rand().
 * Status: used
 */
#pragma once

template <typename T>
T randin(T a, T b) { // Random number in [a, b)
	static random_device rd;
	static mt19937_64 gen(rd());
	uniform_int_distribution<T> dis(a, b - 1);
	return dis(gen);
}
