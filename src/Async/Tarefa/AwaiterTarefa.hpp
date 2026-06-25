#pragma once

#include "../Tarefa.hpp"
#include <common.hpp>

class Escalonador;

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
  inline bool await_ready() const noexcept { return false; }
  std::coroutine_handle<> await_suspend(std::coroutine_handle<>) const noexcept;
  SubTarefaReturnT await_resume();
};

#include "AwaiterTarefa.tpp"