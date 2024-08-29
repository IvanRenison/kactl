/**
 * Author: Iván Renison
 * Date: 2024-08-29
 * License: CC0
 * Source: notebook el vasito
 * Description: Returns at witch side of the plane defined by $a$, $b$ and $c$ the point $p$ is.
 * If the point is on the plane 0 is returned, else 1 or -1.
 * $a$, $b$ and $c$ must not be collinear. For double add epsilon checks.
 * Status: Problem tested
 */
#pragma once

#include "Point.h"

template<class P>
ll sideOf(P a, P b, P c, P p) {
	ll x = (b-a).cross(c-a).dot(p-a);
	return (x > 0) - (x < 0);
}
