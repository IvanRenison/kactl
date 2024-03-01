/**
 * Author: Pietro Palombini
 * Date: 2024-03-
 * License: CC0
 * Source: folklore
 * Description: Finds MST of weighted graph. Returns only cost by default.
 * Time: $O(m)$
 * Status: Tested on https://cses.fi/problemset/result/8586074/
 */
#pragma once

#include "../data-structures/UnionFind.h"

ll kruskal(vector<pair<ll, ii>>& es, ll n) {
    sort(ALL(es));
    UF uf(n);
    ll r = 0;
    for (auto [w, p] : es) {
        auto [u, v] = p;
        if (uf.join(u, v)) {
            r += w;
            // {w, {u, v}} is in MST
        }
    }
    return r;
}
