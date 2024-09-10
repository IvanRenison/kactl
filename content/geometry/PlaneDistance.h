/**
 * Author: Iván Renison
 * Date: 2024-08-29
 * License: CC0
 * Source: notebook el vasito
 * Description:
 * Returns the signed distance between point $p$ and the plane containing points $a$, $b$ and $c$.
 * $a$, $b$ and $c$ must not be collinear.
 * Status: untested
 */
#pragma once

#include "Point3D.h"

template<class P>
double planeDist(P a, P b, P c, P p) {
	P n = (b-a).cross(c-a);
	return (double)n.dot(p-a) / n.dist();
}
