#pragma once

#include "AsyncTCPWindows.hpp"

#include <system_error>
#include <optional>
#include <forward_list>

#include "Escalonavel.hpp"

class AsyncTCP::DNSSolve {
private:
  struct Token {
    OVERLAPPED ov{};
    PADDRINFOEXW pResult{};
    DNSSolve* ctx{nullptr};
  };

public:
  class Err {
  public:
    enum Tipo {
      NENHUM,
      WSA,
      TIMEOUT,
      ABORT,
      CRIT
    };

  private:
    Tipo _tipo;
    int _cod;
  
  public:
    Err(Tipo tipo = NENHUM, int cod = 0) : _tipo{tipo}, _cod{0} {}
    inline Tipo tipo() { return _tipo; }
    inline std::string string() {
      switch (_tipo) {
      case WSA: return std::system_category().message(_cod);
      case TIMEOUT: return "Timeout da operação";
      case ABORT: return "Operação abortada";
      case CRIT: return "Erro crítico";
      default: return "Nenhum erro";
      }
    }
    inline bool existe() { return _tipo != NENHUM; }
  };

private:
  Err _erro{};

  std::forward_list<EnderecoIP> &_enderecos_ip;
  AsyncTCP &_asocket;
  const std::string &_peer_hostname;
  Token _contexto{};
  Escalonavel _esc;


  static void CALLBACK callbackResumir(DWORD dwError, DWORD dwBytes, LPWSAOVERLAPPED lpOverlapped);

  void moverLista();

public:
  DNSSolve(std::forward_list<EnderecoIP> &enderecos_ip, AsyncTCP &asocket, const std::string &peer_hostname);
  DNSSolve(const DNSSolve &) = delete;
  DNSSolve(DNSSolve &&) = delete;
  DNSSolve &operator=(const DNSSolve &) = delete;
  DNSSolve &operator=(DNSSolve &&) = delete;
  ~DNSSolve();

  bool await_ready();
  void await_suspend(std::coroutine_handle<> crth);
  Err await_resume();
};