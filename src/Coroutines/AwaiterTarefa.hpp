#pragma once

#include "Tarefa.hpp"
#include "Escalonador.hpp"
#include <common.hpp>

template <typename TarefaReturnT>
template <typename SubTarefaReturnT>
class Tarefa<TarefaReturnT>::AwaiterTarefa {
  Escalonador *_escalonador;
  std::coroutine_handle<typename Tarefa<SubTarefaReturnT>::Promise> _crth_subtarefa;

public:
  AwaiterTarefa(
    std::coroutine_handle<
      typename Tarefa<SubTarefaReturnT>::Promise
    > crth_subtarefa,
    Escalonador *escalonador
  ) noexcept;
  COLETOR_DELETE_MOVE_COPY(AwaiterTarefa)
  ~AwaiterTarefa() noexcept = default;
  inline bool await_ready() { return false; }
  std::coroutine_handle<> await_suspend(std::coroutine_handle<>);
  SubTarefaReturnT await_resume();
};



template <typename TarefaReturnT>
template <typename SubTarefaReturnT>
Tarefa<TarefaReturnT>::AwaiterTarefa<SubTarefaReturnT>::AwaiterTarefa(
  std::coroutine_handle<
    typename Tarefa<SubTarefaReturnT>::Promise
  > crth_subtarefa,
  Escalonador *escalonador
) noexcept :
_crth_subtarefa{crth_subtarefa},
_escalonador{escalonador} {}



template <typename TarefaReturnT>
template <typename SubTarefaReturnT>
std::coroutine_handle<> Tarefa<TarefaReturnT>::AwaiterTarefa<SubTarefaReturnT>::await_suspend(std::coroutine_handle<>) {
  // Tentar obter uma próxima co-rotina para resumir, realizando transferência simétrica.
  auto prox_crth = _escalonador->desenfileirar();
  // Caso não tenhamos obtido nenhuma co-rotina, transferir para a própria subtarefa.
  if (!prox_crth) return _crth_subtarefa;

  // Caso tenhamos conseguido desenfileirar, escalonar a subtarefa e transferir para a próxima
  _escalonador->enfileirar(_crth_subtarefa);
  return *prox_crth;
}



template <typename TarefaReturnT>
template <typename SubTarefaReturnT>
SubTarefaReturnT Tarefa<TarefaReturnT>::AwaiterTarefa<SubTarefaReturnT>::await_resume() {
  if constexpr (!Tarefa<SubTarefaReturnT>::Promise::retorna_void) {
    auto ret = _crth_subtarefa.promise().extrairValorRetorno();
    _crth_subtarefa.destroy();
    return ret;
  } else {
    _crth_subtarefa.destroy();
  }
}