#pragma once

#include <string>
#include <span>

class EnderecoIP {
public:
  enum Tipo {
    v4,
    v6
  };

private:
  Tipo _tipo;
  std::vector<uint8_t> _endereco;

public:
  EnderecoIP(Tipo tipo, uint8_t *addr, size_t len) : _tipo{tipo}, _endereco(addr, addr + len) {}
  inline Tipo tipo() { return _tipo; }
  inline std::span<const uint8_t> endereco() { return {_endereco.data(), _endereco.size()}; }
};