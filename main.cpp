#include "vdev.hpp"

#include <iostream>

using namespace std;

union Register {
  int rValue;
};

class Machine {
  DEFINE_DEVICE(Machine);

 public:
  DEFINE_RW_PROPERTY(int, rA,
                     [] {
                       cout << "get \n";
                       return 9;
                     },
                     [](int n) {
                       cout << "int = " << n << "\n";
                     });

  DEFINE_RO_PROPERTY(int, rB, [&] { AlarmRead(); return 9999; });
  DEFINE_WO_PROPERTY(int, rC, [](int v) {
      cout << "WO <= " << v << '\n';
    });

  DEFINE_RW_PROPERTY(Register, rD, [&] {
      AlarmRead();
      cout << "Reg <= " << '\n';
      return Register{0};
    },
    [&](auto n) {
      AlarmWrite();
      cout << "Reg = " << n.rValue << "\n";
    });

  void AlarmRead() const {
    cout << "Read\n";
  }

  void AlarmWrite() const {
    cout << "Write\n";
  }

 public:
  int something_;
};

int main() {
  Machine foo;
  foo.rA = foo.rA + 10;
  cout << foo.rB << '\n';
  foo.rC = 10000;
  Register d = foo.rD;
  cout << d.rValue << '\n';
  foo.rD = {100};
  foo.get_rA();
  return 0;
}
