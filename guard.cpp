#include "guard.hpp"

#include <cassert>

int main() {
  int x = 99;
  {
    auto _ = makeBackupGuard(x);
    x = 100;
  }
  assert(x == 99);

  int &y = x;
  {
    auto _ = makeBackupGuard(y);
    y = 99;
  }
  assert(x == 99);

  int* p = &x;
  {
    auto _ = makeBackupGuard(p);
    *p = 100;
  }
  assert(x == 99);

  int** pp = &p;
  {
    auto _ = makeBackupGuard(pp);
    **pp = 100;
  }
  assert(x == 99);

  struct non_copiable {
    non_copiable() = default;
    non_copiable(const non_copiable&) = delete;
    int x;
  };

  non_copiable nc{10};
  non_copiable* pnc = &nc;
  {
    auto _ = makeBackupGuard(pnc);
    pnc->x = 100;
  }
  assert(nc.x == 10);

  return 0;
}
