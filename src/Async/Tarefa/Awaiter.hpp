#pragma once

#include <Async/Tarefa.hpp>
#include <common.hpp>

namespace Async {

class Escalonador;

template <typename ReturnT>
template <typename FilhaReturnT>
class Tarefa<ReturnT>::Awaiter {
  std::coroutine_handle<typename Tarefa<FilhaReturnT>::Promise> _crth_filha;

public:
  explicit Awaiter(std::coroutine_handle<typename Tarefa<FilhaReturnT>::Promise> crth_filha) noexcept;
  COLETOR_DELETE_MOVE_COPY(Awaiter)
  ~Awaiter() noexcept = default;
  inline bool await_ready() const noexcept { return false; }
  std::coroutine_handle<> await_suspend(std::coroutine_handle<>) const noexcept;
  FilhaReturnT await_resume();
};

}

#include "Awaiter.tpp"