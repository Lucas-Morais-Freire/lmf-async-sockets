#include "AwaiterFinal.hpp"
#include <iostream>

template <typename ReturnT>
Tarefa<ReturnT>::AwaiterFinal::AwaiterFinal(std::coroutine_handle<> crth_mae) noexcept :
_crth_mae{crth_mae} {}



template <typename ReturnT>
std::coroutine_handle<> Tarefa<ReturnT>::AwaiterFinal::await_suspend(std::coroutine_handle<typename Tarefa<ReturnT>::Promise> crth) const noexcept {
  // Fazer cópias locais pois o ponteiro `this` pode ser invalidado após enfileirarmos `crth` ou `_crth_mae`.
  std::coroutine_handle<> crth_mae    = _crth_mae;
  Escalonador            *escalonador = crth.promise()._escalonador;
  
  // Tentar obter uma próxima co-rotina para resumir
  auto prox_crth = escalonador->desenfileirar();
  if (!prox_crth) {
    // Caso o escalonador não tenha achado ninguém para transferir a execução, checaremos se temos uma mãe.
    // A mãe será responsável por nos consumir, então não enfileiraremos o nosso `crth`.
    if (crth_mae) return crth_mae;

    // Caso não tenhamos uma mãe, iremos apenas nos enfileirar para que o escalonador nos consuma.
    escalonador->enfileirar(crth);

    // Quebrar a corrente de transferências e retornar ao escalonador
    return std::noop_coroutine();
  }

  // Caso o escalonador tenha achado alguém pra transferir a execução, também checaremos se temos uma mãe.
  // Neste caso, apenas a enfileiraremos. Eventualmente o escalonador irá executá-la para extrair o retorno
  // e nos destruir
  if (crth_mae) escalonador->enfileirar(crth_mae);

  return prox_crth;
}