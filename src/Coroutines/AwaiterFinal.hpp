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
  inline void await_resume() {};
};

#include "AwaiterFinal.tpp"