#pragma once

#include <string>

class Err {
protected:
  int _tipo;
  enum TipoCod {
    UL
  };
  union _cod {
    unsigned long ul;
    
  };

public:
  Err(int tipo, int cod) : _tipo{tipo}, _cod{cod} {}
  template<typename T>
  inline T::Tipo tipo() { return _tipo; }
  virtual std::string_view string() = 0;
  virtual bool existe() = 0;
};