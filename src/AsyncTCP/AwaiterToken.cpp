#include "AwaiterToken.hpp"

AwaiterToken::AwaiterToken(void *ctx) :
_ctx{ctx} {}



void AwaiterToken::setResumivel(std::coroutine_handle<> crth) {
  // Caso o estado atual seja nullptr, substituir pelo handle.
  _crth = crth;
  bool esperado = false;

  if (_escalonavel.compare_exchange_strong(
    esperado,
    true,
    // (Sucesso) Publicar o update de `_crth` para `setFinalizado` enxergar
    std::memory_order_release,
    // (Falha)   Não introduzir cercas
    std::memory_order_relaxed
  )) return;

  // Caso a co-rotina já seja escalonável, escalonar
  crth.resume();
}



void AwaiterToken::setFinalizada() {
  bool esperado = false;

  if (_escalonavel.compare_exchange_strong(
    esperado,
    true,
    // (Sucesso) Não introduzir cercas
    std::memory_order_relaxed,
    // (Falha)   Adquire releases feitos por `setResumivel` para carregar o valor de `_crth`
    std::memory_order_acquire
  )) return;

  // Caso a co-rotina já seja escalonável, escalonar
  _crth.resume();
}