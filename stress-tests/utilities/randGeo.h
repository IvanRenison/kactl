#pragma once

#include "../../content/geometry/Point.h"

template <class T=double>
Point<T> randIntPt(ll lim) {
	return Point<T>(rand()%(lim*2) - lim, rand()%(lim*2)-lim);
}

template <class T=double>
Point<T> randDoublePt(ll lim) {
	static default_random_engine re;
	uniform_real_distribution<T> unif(-lim, lim);
	return Point<T>(unif(re), unif(re));
}
