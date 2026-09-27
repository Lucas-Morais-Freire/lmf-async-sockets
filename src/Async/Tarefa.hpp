#pragma once

#include <coroutine>
#include <exception>
#include <optional>

// 1st-party
#include <common.hpp>

namespace Async {

template <typename ReturnT>
class Promise;


/** @brief Wrapper para Promise<ReturnT> */
template <typename ReturnT = void>
class Tarefa {
  friend class PromiseBase;
  friend class Escalonador;

public:
  template <typename FilhaReturnT>
  class Awaiter;
  class AwaiterFinal;
  using promise_type = Promise<ReturnT>;

private:
  std::coroutine_handle<Promise<ReturnT>> _crth;

public:
  inline explicit Tarefa(std::coroutine_handle<Promise<ReturnT>> crth) noexcept : _crth{crth} {}
  inline ~Tarefa() noexcept = default;
};

}

#include "Tarefa/Awaiter.hpp"
#include "Tarefa/AwaiterFinal.hpp"
#include "Tarefa/Promise.hpp"
#include "Tarefa/Promise.tpp"
#include "Tarefa/Awaiter.tpp"
#include "Tarefa/AwaiterFinal.tpp"