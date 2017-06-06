#ifndef __GUARD_HPP
#define __GUARD_HPP

#include <algorithm>
#include <utility>
#include <type_traits>
#include <tuple>

template<class T, typename = void>
class BackupGuard {
 public:
  BackupGuard(T& v) : origin_(v), value_(v) {}

  ~BackupGuard() {
    origin_ = std::move(value_);
  }

 private:
  T& origin_;
  T value_;
};

template<class T>
class BackupGuard<T, std::enable_if_t<std::is_pointer<T>::value &&
                                      std::is_copy_constructible<std::remove_pointer_t<T>>::value>> {
  using pointee_type = std::remove_pointer_t<T>;
 public:
  BackupGuard(T& v) : value_(*v) {}

 private:
  BackupGuard<pointee_type> value_;
};

template<class T>
class BackupGuard<T, std::enable_if_t<std::is_pointer<T>::value &&
                                      !std::is_copy_constructible<std::remove_pointer_t<T>>::value &&
                                      std::is_default_constructible<std::remove_pointer_t<T>>::value>> {
  using pointee_type = std::remove_pointer_t<T>;
 public:
  BackupGuard(T& v) : origin_(v), pointed_(v) {
    v = &value_;
  }

  ~BackupGuard() {
    origin_ = pointed_;
  }

 private:
  T& origin_;
  T pointed_;
  pointee_type value_{};
};

template<class T>
class BackupGuard<T, std::enable_if_t<std::is_pointer<T>::value &&
                                      !std::is_copy_constructible<std::remove_pointer_t<T>>::value &&
                                      !std::is_default_constructible<std::remove_pointer_t<T>>::value>> {
  static_assert("no backup can be created.");
};

template<class... Ts>
std::tuple<BackupGuard<Ts>...> makeBackupGuard(Ts&... vs) {
  return {vs...};
}

#endif // __GUARD_HPP
