#include "Awaiter.hpp"

#include <Async/Escalonador.hpp>

namespace Async {

Trava::Awaiter::Awaiter(Trava &trava) noexcept :
_trava{trava},
_next{nullptr},
_escalonador{nullptr} {}



Trava::Awaiter::Awaiter(Awaiter &&other) noexcept :
_trava{other._trava},
_next{other._next},
_escalonador{other._escalonador} {
  other._next = nullptr;
  other._escalonador = nullptr;
}



bool Trava::Awaiter::await_ready() {
  // Tentar adquirir trava. A trava será imediatamente adquirida se o valor de
  // `_trava._lista_awaiters` for `nullptr`
  return _trava._lista_awaiters.compare_exchange_strong(
    _next, this,
    // Sucesso: adquirir modificações feitas antes da última liberação da trava e
    // publicar os dados deste awaiter
    std::memory_order_acq_rel,
    // Falha: sem tratamento. `_next` assume o valor atualizado da cabeça da lista
    std::memory_order_relaxed
  );
}



std::coroutine_handle<> Trava::Awaiter::await_suspend(std::coroutine_handle<> crth) {
  // Se formos suspender, precisamos nos colocar na fila da trava
  while (true) {
    if (_next == nullptr) {
      // Caso a trava tenha sido liberada, tentar se colocar na fila atomicamente se a
      // cabeça da fila não mudou desde o último CAS (seja neste laço ou aquele em `await_ready`)
      if (_trava._lista_awaiters.compare_exchange_weak(
        _next, this,
        // Sucesso: adquirir modificações feitas desde a última liberação da trava e
        // publicar os dados deste awaiter
        std::memory_order_acq_rel,
        // Falha: sem tratamento. `_next` assume o valor atualizado da cabeça da lista
        std::memory_order_relaxed
      )) return crth;

    } else {
      // Caso a trava ainda esteja bloqueada, tentar se colocar na fila atomicamente
      // se a cabeça da fila não mudou desde o úlimo CAS (seja neste laço ou aquele em `await_ready`)
      if (_trava._lista_awaiters.compare_exchange_weak(
        _next, this,
        // Sucesso: publicar os dados deste awaiter e suspender
        std::memory_order_release,
        // Falha: sem tratamento, `_next` assume o valor atualizado da cabeça da lista
        std::memory_order_relaxed
      )) return _escalonador->desenfileirar();
    }
  }
}

}