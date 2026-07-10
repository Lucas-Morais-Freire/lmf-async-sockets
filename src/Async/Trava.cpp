#include "Trava.hpp"
#include "Trava/Awaiter.hpp"
#include <Async/Escalonador.hpp>

namespace Async {

Trava::Trava() noexcept :
_lista_novos_awaiters{VAZIA},
_fila_awaiters_cabeca{nullptr},
_fila_awaiters_cauda{nullptr} {}



Trava::Awaiter Trava::adquirir() noexcept { return Awaiter{*this}; }



void Trava::liberar() noexcept {
  // Esta variável recebe o ponteiro para a lista de novos awaiters caso existam
  Awaiter *cabeca_lista_novos_awaiters{TRAVADO_SEM_NOVOS_AWAITERS};
  /*
  Caso base: não há awaiters esperando para serem desbloqueados
  A thread que fez esta lista de awaiters foi a última a liberar o mutex.
  as mudanças feitas por ela são feitas visíveis a esta thread pois há sempre
  um release feito para qualquer update em `_lista_awaiters` e há sempre
  um acquire após um awaiter adquirir o mutex
  */ 
  Awaiter *proximo_detentor = _fila_awaiters_frente;
  Awaiter *prox_estado = VAZIA;
  // Caso existam awaiters, manter a trava, e remover `proximo_detentor` da lista de awaiters
  if (proximo_detentor) {
    _fila_awaiters_frente = proximo_detentor->_next;
    if (!_fila_awaiters_frente) _fila_awaiters_traseira = nullptr;
    prox_estado = TRAVADO_SEM_NOVOS_AWAITERS;
  }
  if (_lista_novos_awaiters.compare_exchange_strong(
    // Checar se a lista de novos awaiters está vazia
    cabeca_lista_novos_awaiters,
    // se sim, escrever o novo estado de acordo com o valor de `prox_estado`
    prox_estado,
    // Como a lista de awaiters for alterada, publicar esta mudança com um release
    std::memory_order_release,
    // Caso a lista de novos awaiters não esteja vazia, apenas escrever
    // seu valor em `cabeca_lista_novos_awaiters`
    std::memory_order_relaxed
  )) {
    if (proximo_detentor) proximo_detentor->_escalonador->enfileirar(proximo_detentor->_crth);
    return;
  }

  // todo: reverter a lista de novos awaiters e colocá-la no fim da lista de awaiters
}

}