#include "../utilities/template.h"

namespace M {
#include "../../content/number-theory/Mobius.h"
}
namespace E {
#include "../../content/number-theory/Eratosthenes.h"
}
const ll L = 1e6;

int main() {
  assert(L == M::L);
  assert(L <= E::L);
  E::sieve();
  M::calculateMu();

  assert(M::mu[0] == 0);
  fore(i, 1, L) {
    vector<ii> f = E::fact(i);
    bool squarefree = true;
    for (auto [p, e] : f) {
      if (e > 1) {
        squarefree = false;
        break;
      }
    }
    if (!squarefree) {
      assert(M::mu[i] == 0);
    } else if (SZ(f) % 2 == 0) {
      assert(M::mu[i] == 1);
    } else {
      assert(M::mu[i] == -1);
    }
  }
}
