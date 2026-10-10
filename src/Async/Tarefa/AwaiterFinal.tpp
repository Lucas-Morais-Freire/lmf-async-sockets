#pragma once

#ifndef ASYNC_H
#error "Você deve incluir este arquivo via \"Async/Async.hpp\""
#endif

namespace Async {

template <typename ReturnT>
Tarefa<ReturnT>::AwaiterFinal::AwaiterFinal(std::coroutine_handle<> crth_mae) noexcept :
_crth_mae{crth_mae} {}



template <typename ReturnT>
std::coroutine_handle<> Tarefa<ReturnT>::AwaiterFinal::await_suspend(std::coroutine_handle<> crth) const noexcept {
  // Fazer cópia local pois o ponteiro `this` pode ser invalidado após enfileirarmos `crth` ou `_crth_mae`.
  std::coroutine_handle<> crth_mae = _crth_mae;
  Escalonador         *escalonador = PromiseBase::from_crth(crth).escalonador();
  
  // Tentar obter uma próxima corrotina para resumir com o escalonador desta corrotina
  auto prox_crth = escalonador->desenfileirar();
  if (prox_crth == std::noop_coroutine()) {
    // Caso o escalonador não tenha achado ninguém para transferir a execução, checaremos se temos uma mãe.
    // A mãe será responsável por nos consumir, então não enfileiraremos o nosso `crth`.
    if (crth_mae) return crth_mae;
    
    // Caso não tenhamos uma mãe, iremos apenas nos enfileirar para que ele nos consuma.
    escalonador->enfileirar(crth);
    
    // Quebrar a corrente de transferências e retornar ao escalonador
    return prox_crth;
  }
  
  // Caso o escalonador tenha achado alguém pra transferir a execução, também checaremos se temos uma mãe.
  // Neste caso, apenas a enfileiraremos. Eventualmente o escalonador irá executá-la para extrair o retorno
  // e nos consumir
  if (crth_mae) escalonador->enfileirar(crth_mae);

  return prox_crth;
}

}
