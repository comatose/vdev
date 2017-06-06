#define BOOST_HANA_CONFIG_ENABLE_STRING_UDL
#include <boost/hana.hpp>

#include <functional>
#include <vector>
#include <iostream>

namespace hana = boost::hana;

using Callback = std::function<void()>;

template <class EventMap>
struct event_system {
  EventMap map_;

  template <typename Event, typename F>
  void on(Event e, F callback) {
    auto is_known_event = hana::contains(map_, e);
    static_assert(is_known_event,
                  "trying to add a callback to an unknown event");

    map_[e].push_back(callback);
  }

  template <typename Event>
  void trigger(Event e) const {
    auto is_known_event = hana::contains(map_, e);
    static_assert(is_known_event,
                  "trying to trigger an unknown event");

    for (auto& callback : map_[e])
      callback();
  }
};

template <typename ...Events>
auto  make_event_system(Events ...events) {
  auto em = hana::make_map(hana::make_pair(events, std::vector<Callback>{})...);
  return event_system<decltype(em)>{std::move(em)};
}

int main() {
  using namespace hana::literals;

  auto events = make_event_system("foo"_s, "bar"_s, "baz"_s);

  events.on("foo"_s, []() { std::cout << "foo triggered!" << '\n'; });
  events.on("foo"_s, []() { std::cout << "foo again!" << '\n'; });
  events.on("bar"_s, []() { std::cout << "bar triggered!" << '\n'; });
  events.on("baz"_s, []() { std::cout << "baz triggered!" << '\n'; });
  // events.on("unknown"_s, []() {}); // compiler error!

  events.trigger("foo"_s); // no overhead
  events.trigger("baz"_s);
  // events.trigger("unknown"_s); // compiler error!
}
