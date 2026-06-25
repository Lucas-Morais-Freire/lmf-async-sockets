#pragma once

// std
#include <coroutine>
#include <chrono>

// 1st-party
#include "CategoriaPrioridade.hpp"

template <typename TarefaReturnT = void>
class Tarefa {
  friend class Escalonador;

private:
  template <typename SubTarefaReturnT>
  class AwaiterTarefaBase;
  template <typename SubTarefaReturnT>
  class AwaiterTarefa;
  
  class AwaiterFinalBase;
  class AwaiterFinal;
  
public:
  template <typename PromiseDerivadaT>
  class PromiseBase;
  class Promise;
  using promise_type = Promise;

protected:
  std::coroutine_handle<Promise> _crth;

public:
  explicit Tarefa(std::coroutine_handle<Promise> crth);
};

#include "Tarefa/Promise.hpp"

template <typename TarefaReturnT>
Tarefa<TarefaReturnT>::Tarefa(std::coroutine_handle<Promise> crth) :
_crth{crth} {}