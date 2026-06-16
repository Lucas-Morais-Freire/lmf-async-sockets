#pragma once

#include "AsyncTCPWindows.hpp"

#include <system_error>
#include <optional>
#include <forward_list>

#include "CrtToken.hpp"
#include "Err/DNSLookupErr.hpp"

class AsyncTCP::DNSLookup {
private:
  struct AwaiterTokenWrapper {
    OVERLAPPED _ov;
    PADDRINFOEXW _pResult;
    AwaiterToken *_token;
  };

private:
  // Padrões de co-rotina
  DNSLookupErr _erro;
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
  DNSLookupErr await_resume();
};