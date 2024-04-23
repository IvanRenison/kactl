/**
 * Author: libstdc++ authors
 * Date: 2024-04-20
 * License: CC0
 * Source: notebook El Vasito
 * Description: Sequence with O(log(n)) random access, insert,
 * erase at any position. Very high constant factors.
 * Usage:
 *  s.push_back(x);
 *  s.insert(i,r) // insert rope r at position i
 *  s.erase(i,k) // erase subsequence [i,i+k)
 *  s.substr(i,k) // return a new rope
 *  s[i] // access ith element (cannot modify)
 *  s.mutable_reference_at(i)
 *  s.begin() and s.end() are const iterators
 *  s.mutable_begin(), s.mutable_end()
*/

#include <ext/rope> /** keep-include */
using namespace __gnu_cxx;
rope<ll> s;
