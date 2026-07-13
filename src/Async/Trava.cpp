#include "Trava.hpp"
#include "Trava/Awaiter.hpp"
#include <Async/Escalonador.hpp>
#include <Async/Tarefa.hpp>

namespace Async {

Trava::Trava() noexcept :
_lista_novos_awaiters{VAZIA},
_lista_awaiters{nullptr} {}



Trava::Awaiter Trava::adquirir() noexcept { return Awaiter{*this}; }



void Trava::liberar() noexcept {
  // Verificar se há alguém esperando na lista de awaiters
  if (_lista_awaiters == nullptr) {
    // Caso não tenha ninguém esperando, teremos duas possibilidades:

    // 1) Caso não exista ningúem na lista de novos awaiters, apenas liberar a trava
    Awaiter *cabeca_novos_awaiters{TRAVADA_SEM_NOVOS_AWAITERS};
    if (_lista_novos_awaiters.compare_exchange_strong(
      cabeca_novos_awaiters,
      VAZIA,
      // Sucesso: publicar todas as mudanças que esta thread fez enquanto era detentora da trava
      std::memory_order_release,
      // Falha: fazer nada
      std::memory_order_relaxed
    )) return;

    // 2) Caso exista alguém na lista de novos awaiters, transferir esta lista para a principal.
    // Ao escrever `TRAVADA_SEM_NOVOS_AWAITERS` nela, uma nova fila começará a se formar de acordo
    // com o algoritmo em `Async::Trava::Awaiter::await_suspend`.
    // Precisamos de semânticas de `acquire` para que possamos adquirir os dados publicados durante esse
    // `await_suspend`
    cabeca_novos_awaiters = _lista_novos_awaiters.exchange(TRAVADA_SEM_NOVOS_AWAITERS, std::memory_order_acquire);

    // Inverter a linkedlist. Isto é importante para manter a trava FIFO (por conseguinte, justa, o que é de extrema importância)
    Awaiter *temp = nullptr;
    while (cabeca_novos_awaiters != nullptr) {
      Awaiter *next = cabeca_novos_awaiters->_next;
      cabeca_novos_awaiters->_next = temp;
      temp = cabeca_novos_awaiters;
      cabeca_novos_awaiters = next;
    }

    // A nova cabeça de `_lista_awaiters` agora se encontra em `temp`
    _lista_awaiters = temp;
  }

  // A partir daqui, há garantia de que `_lista_awaiters` possui um ponteiro válido.

  Awaiter *proximo_detentor = _lista_awaiters;
  _lista_awaiters = _lista_awaiters->_next;

  // Não é necessário publicar nada porque a trava do escalonador já provê toda a sincronização
  // necessária (desbloqueio do mutex após enfileirar funciona como um 'release', e bloqueio do
  // mutex ao desenfileirar funciona como um 'acquire')
  proximo_detentor->_crth.promise().escalonador()->enfileirar(proximo_detentor->_crth);
}

}