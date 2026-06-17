#pragma once

#include <Err/Err.hpp>
#include <system_error>

#include <minwindef.h>

class AsyncTCP_DNSLookup_Err : public Err {
private:
  std::string_view _funcao;

public:
  enum Tipo : int {NENHUM, CONVERSAO_STRING, GETADDRINFO, TIMEOUT, ABORT};

  template <typename T = int>
  requires std::is_integral_v<T>
  AsyncTCP_DNSLookup_Err(std::string_view funcao = "", Tipo tipo = NENHUM, T cod = 0) : ::Err(tipo, cod), _funcao{funcao} {}
  std::string string() override {
    std::string ret{_funcao};
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