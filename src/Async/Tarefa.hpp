#pragma once

// std
#include <coroutine>
#include <chrono>

namespace Async {

template <typename ReturnT = void>
class Tarefa {
  friend class Escalonador;

  template <typename AnyReturnT>
  friend class Tarefa<AnyReturnT>::PromiseBase;

public:
  template <typename FilhaReturnT>
  class Awaiter;
  class AwaiterFinal;
  class PromiseBase;
  class Promise;
  using promise_type = Promise;

private:
  std::coroutine_handle<Promise> _crth;

public:
  inline explicit Tarefa(std::coroutine_handle<Promise> crth) : _crth{crth} {}
};

}

#include "Tarefa/Promise.hpp"