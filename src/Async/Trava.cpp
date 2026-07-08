#include "Trava.hpp"
#include "Trava/Awaiter.hpp"
#include <Async/Escalonador.hpp>

namespace Async {

Trava::Trava() noexcept :
_fila_awaiters_cabeca{nullptr},
_fila_awaiters_cauda{nullptr} {}



Trava::Awaiter &Trava::adquirir() noexcept { return *(new Awaiter{*this}); }



void Trava::liberar() noexcept {
  while (true) {
    Awaiter *awaiter_candidato = _lista_awaiters.load(std::memory_order_acquire);
    Awaiter *proxima_cabeca = awaiter_candidato->_next;
    if (_lista_awaiters.compare_exchange_weak(
      awaiter_candidato, proxima_cabeca,
      std::memory_order_relaxed,
      std::memory_order_relaxed
    )) {
      if (awaiter_candidato)
      awaiter_candidato->_escalonador->enfileirar(awaiter_candidato->_crth);
      break;
    }
  }
}

}