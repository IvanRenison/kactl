#pragma once
#include "../utilities/template.h"

// Check if the polygon is convex and counterclockwise
template<class P>
bool isConvexCCW(vector<P>& p) {
	ll n = SZ(p);
	fore(i, 0, n) {
		if (sideOf(p[i], p[(i + 1) % n], p[(i + 2) % n]) < 0) {
			return false;
		}
	}
	return true;
}
template<class P>
bool isConvexCCW(vector<P>& p, double eps) {
	ll n = SZ(p);
	fore(i, 0, n) {
		if (sideOf(p[i], p[(i + 1) % n], p[(i + 2) % n], eps) < 0) {
			return false;
		}
	}
	return true;
}

// Test if the two polygons are equal
template<class P>
bool polygonEq(vector<P>& p, vector<P>& q) {
	ll n = SZ(p);
	if (n != SZ(q)) return false;
	if (p == q) return true;
	fore(i, 1, n) {
		bool valid = true;
		fore(j, 0, n) {
			if (!(p[j] == q[(j + i) % n])) {
				valid = false;
				break;
			}
		}
		if (valid) return true;
	}
	return false;
}
template<class P>
bool polygonEq(vector<P>& p, vector<P>& q, double eps) {
	ll n = SZ(p);
	if (n != SZ(q)) return false;
	if (n == 0) return true;
	fore(i, 0, n) {
		bool valid = true;
		fore(j, 0, n) {
			if (abs(p[j].x - q[(j + i) % n].x) > eps || abs(p[j].y - q[(j + i) % n].y) > eps) {
				valid = false;
				break;
			}
		}
		if (valid) return true;
	}
	return false;
}

