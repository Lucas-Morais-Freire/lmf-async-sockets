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
  other._next = Trava::VAZIA;
  other._escalonador = nullptr;
}



bool Trava::Awaiter::await_ready() {
  // Tentar adquirir trava. A trava será imediatamente adquirida se o valor de
  // `_trava._lista_novos_awaiters` for `Trava::VAZIA`
  return _trava._lista_novos_awaiters.compare_exchange_strong(
    _next, Trava::TRAVADO_SEM_NOVOS_AWAITERS,
    // Sucesso: adquirir modificações feitas antes da última liberação da trava
    std::memory_order_acquire,
    // Falha: sem tratamento. `_next` assume o valor atualizado da cabeça da lista
    std::memory_order_relaxed
  );
}



std::coroutine_handle<> Trava::Awaiter::await_suspend(std::coroutine_handle<> crth) {
  // Se formos suspender, precisamos nos colocar na fila de novos awaiters
  while (true) {
    if (_next == Trava::VAZIA) {
      // Caso a trava tenha sido liberada, tentar se colocar na fila de novos awaiters atomicamente se a
      // cabeça da fila não mudou desde o último CAS (seja neste laço ou aquele em `await_ready`)
      if (_trava._lista_novos_awaiters.compare_exchange_weak(
        _next, Trava::TRAVADO_SEM_NOVOS_AWAITERS,
        // Sucesso: adquirir modificações feitas desde a última liberação da trava.
        // Não precisamos fazer um release porque:
        // 1) Esta mesma thread irá desbloquear o mutex, nossos dados são visíveis a ela, ou...
        // 2) ...Esta thread irá suspender, e será resumida por outra thread, processo que envolve
        //    sincronização entre a thread atual e a próxima, tornando os dados visíveis para a próxima
        std::memory_order_acquire,
        // Falha: sem tratamento. `_next` assume o valor atualizado da cabeça da lista
        std::memory_order_relaxed
      )) return crth;

    } else {
      // Caso a trava ainda esteja bloqueada, tentar se colocar na fila atomicamente
      // se a cabeça da fila não mudou desde o úlimo CAS (seja neste laço ou aquele em `await_ready`)
      if (_trava._lista_novos_awaiters.compare_exchange_weak(
        _next, this,
        // Sucesso: publicar os dados deste awaiter e suspender. Este release é necessário
        // porque a thread que for desbloquear o mutex precisará ler nossos dados, e eles precisam
        // estar visíveis para ela
        std::memory_order_release,
        // Falha: sem tratamento, `_next` assume o valor atualizado da cabeça da lista
        std::memory_order_relaxed
      )) return _escalonador->desenfileirar();
    }
  }
}

}