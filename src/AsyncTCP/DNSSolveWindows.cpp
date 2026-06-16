#include "DNSSolveWindows.hpp"

#include <common.hpp>
#include <Utils/Utils.hpp>

void CALLBACK AsyncTCP::DNSSolve::callbackResumir(DWORD dwError, DWORD dwBytes, LPWSAOVERLAPPED lpOverlapped) {
  Token &token  = rCast<Token &>(*lpOverlapped);
  DNSSolve &ctx = *token.ctx;

  if (dwError != NO_ERROR) {
    ctx._erro = {Err::WSA, dwError};
  }

  // Tentar escalonar a co-rotina aqui
  ctx._esc.setFinalizado();
}



AsyncTCP::DNSSolve::DNSSolve(std::forward_list<EnderecoIP> &enderecos_ip, AsyncTCP &asocket, const std::string &peer_hostname) :
_erro{},
_enderecos_ip{enderecos_ip},
_asocket{asocket},
_peer_hostname{peer_hostname},
_contexto{{}, {}, this} {}



AsyncTCP::DNSSolve::~DNSSolve() {}



bool AsyncTCP::DNSSolve::await_ready() {
  // Especificar hints para o DNS lookup
  ADDRINFOEXW hints  = {0};
  hints.ai_family   = AF_UNSPEC;   // IPv4 ou IPv6
  hints.ai_socktype = SOCK_STREAM;

  // Converter para UTF-16 para poder usar GetAddrInfoW
  auto peer_hostname_utf16 = Utils::utf8ToUtf16(_peer_hostname);
  if (!peer_hostname_utf16) {
    _erro = {Err::CRIT, -1};
    return true;
  }

  // Precisamos usar a versão 'W' porque a versão 'A' não suporta timeouts.
  // Fix: adicionar parâmetro de timeout
  INT ret = GetAddrInfoExW(
    peer_hostname_utf16->c_str(), nullptr,
    NS_DNS, nullptr,
    &hints, &_contexto.pResult,
    nullptr, &_contexto.ov,
    callbackResumir, nullptr
  );

  switch (ret) {
  case WSA_IO_PENDING: // Caso onde o awaiter irá mandar a co-rotina suspender.
    return false;
  case NO_ERROR:       // Caso onde o awaiter pode retornar um resultado positivo imediatamente.
    return true;
  default:             // Caso onde o awaiter pode retornar um resultado negativo imediatamente.
    _erro = {Err::WSA, ret};
    return true;
  }
}



void AsyncTCP::DNSSolve::await_suspend(std::coroutine_handle<> crth) {
  // Tentar escalonar a co-rotina aqui
  _esc.setResumivel(crth);
}



AsyncTCP::DNSSolve::Err AsyncTCP::DNSSolve::await_resume() {
  // Caso resumimos e temos erro, apenas retorná-lo.
  if (_erro.existe()) return _erro;

  // Caso não tenhamos tido nenhum erro, preencher a lista com os
  // IPs que a linkedlist encontrou
  _enderecos_ip.clear();
  for (PADDRINFOEXW p = _contexto.pResult; p != nullptr; p = p->ai_next) {
    switch (p->ai_family) {
    case AF_INET:
      _enderecos_ip.emplace_front(
        EnderecoIP::v4,
        rCast<uint8_t *>(&rCast<sockaddr_in *>(p->ai_addr)->sin_addr.s_addr),
        sizeof(in_addr)
      );
      break;
    case AF_INET6:
      _enderecos_ip.emplace_front(
        EnderecoIP::v6,
        rCast<uint8_t *>(&rCast<sockaddr_in6 *>(p->ai_addr)->sin6_addr.s6_addr),
        sizeof(in6_addr)
      );
      break;
    default:
    }
  }

  FreeAddrInfoExW(_contexto.pResult);

  return _erro;
}