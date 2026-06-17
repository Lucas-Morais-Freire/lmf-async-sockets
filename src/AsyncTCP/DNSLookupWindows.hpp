#pragma once

#include "AsyncTCPWindows.hpp"

#include <system_error>
#include <optional>
#include <forward_list>

#include "CrtToken.hpp"
#include <Err/Err.hpp>

class AsyncTCP::DNSLookup {
public:
  class Err : public ::Err {
  private:
    std::string_view _funcao;

  public:
    enum Tipo : int {NENHUM, CONVERSAO_STRING, GETADDRINFO, TIMEOUT, ABORT};

    template <typename T = int>
    requires std::is_integral_v<T>
    Err(std::string_view funcao = "", Tipo tipo = NENHUM, T cod = 0) : ::Err(tipo, cod), _funcao{funcao} {}
    std::string string() override {
      std::string ret{_funcao};
      switch(tipo<Err>()) {
      case NENHUM:           ret += ": Nenhum erro"; break;
      case CONVERSAO_STRING: ret += ": Erro ao converter string de UTF-8 para UTF-16"; break;
      case GETADDRINFO:      ret += ": Erro em GetAddrInfoExW(): " + std::system_category().message(cod<DWORD>()); break;
      case TIMEOUT:          ret += ": Timeout de " + std::to_string(cod<size_t>()) + "ms atingido"; break;
      case ABORT:            ret += ": Operação abortada"; break;
      }

      return ret;
    }
    inline bool existe() override { return tipo<Err>() != NENHUM; }
  };

private:
  struct AwaiterTokenWrapper {
    OVERLAPPED _ov;
    PADDRINFOEXW _pResult;
    AwaiterToken *_token;
  };

private:
  // Padrões de co-rotina
  Err _erro;
  AwaiterToken _token;
  AwaiterTokenWrapper _crt_token_wrapper;

  // Argumentos
  std::forward_list<EnderecoIP> &_enderecos_ip;
  AsyncTCP &_asocket;
  const std::string &_peer_hostname;

  static void CALLBACK callbackResumir(DWORD dwError, DWORD dwBytes, LPWSAOVERLAPPED lpOverlapped);

public:
  DNSLookup(std::forward_list<EnderecoIP> &enderecos_ip, AsyncTCP &asocket, const std::string &peer_hostname);
  DNSLookup(const DNSLookup &) = delete;
  DNSLookup(DNSLookup &&) = delete;
  DNSLookup &operator=(const DNSLookup &) = delete;
  DNSLookup &operator=(DNSLookup &&) = delete;
  ~DNSLookup();

  bool await_ready();
  void await_suspend(std::coroutine_handle<> crth);
  Err await_resume();
};