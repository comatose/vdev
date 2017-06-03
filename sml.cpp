#include <cassert>
#include <boost/sml.hpp>

namespace sml = boost::sml;

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
    using namespace sml;
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

int main() {
  using namespace sml;

  sender s;
  sm<tcp_release> sm{s}; // pass dependencies via ctor...
  assert(sm.is("established"_s));

  sm.process_event(release{}); // complexity O(1) -> jump table
  assert(sm.is("fin wait 1"_s));

  sm.process_event(ack{});
  assert(sm.is("fin wait 2"_s));

  sm.process_event(fin{});
  assert(sm.is("timed wait"_s));

  sm.process_event(timeout{});
  assert(sm.is(X));  // released
}
