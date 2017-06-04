#include "vdev.hpp"

#include <iostream>

#include <cassert>
#include <boost/sml.hpp>
#include <boost/hana.hpp>

namespace sml = boost::sml;
using namespace sml;

namespace hana = boost::hana;

using namespace std;

//dependencies
struct sender {
  template<class T>
  void send(const T&) {}
};

// events
struct ack { bool valid = true; };
struct fin { bool valid = true;};
struct release {};
struct timeout {};

// guards
const auto is_valid = [](const auto& event) { return event.valid; };

// actions
const auto send_fin = [](sender& s) { s.send(fin{}); };
const auto send_ack = [](const auto& event, sender& s) { s.send(event); };

struct tcp_release {
  auto operator()() const {
    /**
     * Initial state: *initial_state
     * Transition DSL: src_state + event [ guard ] / action = dst_state
     */
    return make_transition_table(
        *"established"_s + event<release> / send_fin          = "fin wait 1"_s,
        "fin wait 1"_s  + event<ack> [ is_valid ]            = "fin wait 2"_s,
        "fin wait 2"_s  + event<fin> [ is_valid ] / send_ack = "timed wait"_s,
        "timed wait"_s  + event<timeout> / send_ack          = X
                                 );
  }
};

class Machine {
  DEFINE_DEVICE(Machine);

  sender s;
  sm<tcp_release> sm{s}; // pass dependencies via ctor...

 public:
  DEFINE_RO_PROPERTY(int, rState, [&] {
      if(sm.is("established"_s))
        return 0;
      if(sm.is("fin wait 1"_s))
        return 1;
      if(sm.is("fin wait 2"_s))
        return 2;
      if(sm.is("timed wait"_s))
        return 3;
      if(sm.is(X))
        return 4;  // released
    });

  DEFINE_WO_PROPERTY(int, rEvent, [&] (int e) {
      switch(e) {
        case 0:
          sm.process_event(release{}); // complexity O(1) -> jump table
          break;
        case 1:
          sm.process_event(ack{});
          break;
        case 2:
          sm.process_event(fin{});
          break;
        case 3:
          sm.process_event(timeout{});
          break;
      }
    });
};

template<typename>
struct show_type;

int main() {
  Machine dev;
  assert(dev.rState == 0); // assert(sm.is("established"_s));

  dev.rEvent = 0; // sm.process_event(release{}); // complexity O(1) -> jump table
  assert(dev.rState == 1); // assert(sm.is("fin wait 1"_s));

  dev.rEvent = 1; // sm.process_event(ack{});
  assert(dev.rState == 2); // assert(sm.is("fin wait 2"_s));

  dev.rEvent = 2; // sm.process_event(fin{});
  assert(dev.rState == 3); // assert(sm.is("timed wait"_s));

  dev.rEvent = 3; // sm.process_event(timeout{});
  assert(dev.rState == 4); // assert(sm.is(X));  // released
}
