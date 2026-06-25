#pragma once

#include <Err/Err.hpp>
#include <system_error>
#include <source_location>

#include <minwindef.h>

class AsyncTCP_DNSLookup_Err : public Err {
private:
  std::source_location _loc;

public:
  enum Tipo : int {NENHUM, CONVERSAO_STRING, GETADDRINFO, TIMEOUT, ABORT};

  template <typename T = int>
  requires std::is_integral_v<T>
  AsyncTCP_DNSLookup_Err(Tipo tipo = NENHUM, T cod = 0, std::source_location loc = std::source_location::current()) : ::Err(tipo, cod), _loc{loc} {}
  std::string string() override {
    std::string ret{_loc.function_name()};
    switch(tipo<AsyncTCP_DNSLookup_Err>()) {
    case NENHUM:           ret += ": Nenhum erro"; break;
    case CONVERSAO_STRING: ret += ": Erro ao converter string de UTF-8 para UTF-16"; break;
    case GETADDRINFO:      ret += ": Erro em GetAddrInfoExW(): " + std::system_category().message(cod<DWORD>()); break;
    case TIMEOUT:          ret += ": Timeout de " + std::to_string(cod<size_t>()) + "ms atingido"; break;
    case ABORT:            ret += ": Operação abortada"; break;
    }

    return ret;
  }
  inline bool existe() override { return tipo<AsyncTCP_DNSLookup_Err>() != NENHUM; }
};