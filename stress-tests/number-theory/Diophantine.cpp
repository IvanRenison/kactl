#include "../utilities/template.h"

#include "../../content/number-theory/Diophantine.h"
#include "../../content/various/Randin.h"

int main() {
  ll n = 1e7, mx = 1e9;
  fore(_, 0, n) {
    ll a = randin(-mx, mx+1), b = randin(-mx, mx+1), k = randin(-mx, mx+1);
    ll r = gcd(a, b) * k;
    auto [x, y, dx, dy] = diophantine(a, b, r);
    assert(a * x + b * y == r);
    fore(i, -15, 16)
      assert(a * (x + i * dx) + b * (y + i * dy) == r);
  }
}
