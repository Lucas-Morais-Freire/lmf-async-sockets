#pragma once

#include <Async/Tarefa.hpp>
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
  inline bool await_ready() const noexcept { return false; }
  std::coroutine_handle<> await_suspend(std::coroutine_handle<> crth) const noexcept;
  inline void await_resume() const noexcept {};
};

#include "AwaiterFinal.tpp"