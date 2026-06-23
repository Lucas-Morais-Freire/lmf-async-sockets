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

#include "AwaiterTarefa.tpp"