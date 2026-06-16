#pragma once

#include <Err/Err.hpp>
#include <array>

class DNSLookupErr : public Err {
public:
  enum Tipo {
    NENHUM,
    CONVERSAO_STRING,
    GETADDRINFO,
    TIMEOUT,
    CRIT
  };

private:
  static constexpr std::array<std::string_view, 5> strings = {
    "Nenhum",
    "Erro durante conversão de string",
    "Erro em chamada de GetAddrInfoExW",
    "Timeout",
    "Erro crítico"
  };

public:
  DNSLookupErr(Tipo tipo = NENHUM, int cod = 0) : Err(tipo, cod) {}

  inline std::string_view string() override { return strings[_tipo]; }
  inline bool existe() override { return _tipo != NENHUM; }
};