#pragma once

#include "Tarefa.hpp"
#include <common.hpp>

template <typename TarefaReturnT>
class Tarefa<TarefaReturnT>::AwaiterFinal {
private:
  std::coroutine_handle<> _crth_mae;
  Escalonador *_escalonador;

public:
  AwaiterFinal(std::coroutine_handle<> crth_mae, Escalonador *escalonador) noexcept;
  COLETOR_DELETE_MOVE_COPY(AwaiterFinal)
  inline ~AwaiterFinal() noexcept = default;
  inline bool await_ready() { return false; }
  std::coroutine_handle<> await_suspend(std::coroutine_handle<> crth);
  void await_resume() {};
};



template <typename TarefaReturnT>
Tarefa<TarefaReturnT>::AwaiterFinal::AwaiterFinal(std::coroutine_handle<> crth_mae, Escalonador *escalonador) noexcept :
_crth_mae{crth_mae},
_escalonador{escalonador} {}



template <typename TarefaReturnT>
std::coroutine_handle<> Tarefa<TarefaReturnT>::AwaiterFinal::await_suspend(std::coroutine_handle<> crth) {
  // Fazer cópias locais pois o ponteiro `this` pode ser invalidado após enfileirarmos `crth` ou `_crth_mae`.
  auto escalonador = _escalonador;
  auto crth_mae = _crth_mae;
  
  // Tentar obter uma próxima co-rotina para resumir
  auto prox_crth = escalonador->desenfileirar();
  if (!prox_crth) {
    // Caso o escalonador não tenha achado ninguém para transferir a execução, checaremos se temos uma mãe.
    // A mãe será responsável por nos destruir
    if (crth_mae) return crth_mae;

    // Caso não tenhamos uma mae, iremos apenas nos enfileirar para que o escalonador nos libere
    escalonador->enfileirar(crth);

    // Quebrar a corrente de transferências
    return std::noop_coroutine();
  }

  // Caso o escalonador tenha achado alguém pra transferir a execução, também checaremos se temos uma mãe.
  // Neste caso, apenas a enfileiraremos. Eventualmente o escalonador irá executá-la para extrair o retorno
  // e nos destruir
  if (crth_mae) escalonador->enfileirar(crth_mae);

  return *prox_crth;
}