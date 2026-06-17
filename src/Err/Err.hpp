#pragma once

#include <string>
#include <cstring>

#include <common.hpp>

class Err {
private:
  int _tipo;
  size_t _cod;
  
protected:
  template <typename T>
  requires std::is_integral_v<T>
  T cod() {
    union {
      size_t full;
      T small;
    } u;
    u.full = _cod;
    return u.small;
  }

  template <typename T = int>
  requires std::is_integral_v<T>
  Err(int tipo, T cod) : _tipo{tipo}, _cod{0} {
    std::memcpy(&_cod, &cod, sizeof(T));
  }
  template<typename T>
  requires (
    std::is_enum_v<typename T::Tipo> &&
    std::is_same_v<std::underlying_type_t<typename T::Tipo>, int>
  )
  inline T::Tipo tipo() { return sCast<T::Tipo>(_tipo); }
  virtual std::string string() = 0;
  virtual bool existe() = 0;
};