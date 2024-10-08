/**
 * Author: Chen Xing
 * Date: 2009-03-13
 * License: CC0
 * Source: Conversation with Håkan and Ulf.
 * Description: Example structures and functions that doesn't really do
 *  anything. Latex commands are supported here, though! Like this: $2^2=3$
 * Usage:
 *  ll res = exampleFunction1(mice, cats, catsCount);
 *  ll random = exampleFunction2();
 * Time: O(M * N^4) where M = cats, N = mice
 * Memory: O(2^N) where N = cats
 * Status: Untested
 */
#pragma once

/// Headers and macros from contest/template.h are automatically included.

#include "Example.h" /// Header name will be kept in final list.

struct ExampleStructure {
	ll alpha;
	ii nodes;
	vi edges;
};

ll exampleFunction1(vi mice, ll* cats, ll catsCount) {
	/// This is a stupid comment that will be removed.
	ll sum = 0;

	fore(i, 1, catsCount) {
		// This comment will be kept!
		for (auto it : mice) {
			/// This will be removed.
			// And this kept.
			sum += cats[i] * it;
		}
	}
	// Use tab indent and keep a line width of at most 63 chars,
	// where tabs count as 2 characters wide.
	//<---------------------Maximum width----------------------->
	return sum;
}

ll exampleFunction2() {
	return rand();
}

ll exampleFunction3(ll a, ll b, ll c) {
	return exampleFunction2() * a * b * c;
}
