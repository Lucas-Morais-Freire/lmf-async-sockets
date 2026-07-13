#pragma once

#include <Async/Tarefa.hpp>
#include <common.hpp>

namespace Async {

class Escalonador;

template <typename ReturnT>
template <typename FilhaReturnT>
class Tarefa<ReturnT>::Awaiter {
  std::coroutine_handle<Promise<FilhaReturnT>> _crth_filha;

public:
  explicit Awaiter(std::coroutine_handle<Promise<FilhaReturnT>> crth_filha) noexcept;
  COLETOR_DELETE_MOVE_COPY(Awaiter)
  ~Awaiter() noexcept = default;
  inline bool await_ready() const noexcept { return false; }
  std::coroutine_handle<PromiseBase> await_suspend(std::coroutine_handle<Promise<ReturnT>>) const noexcept;
  FilhaReturnT await_resume();
};

}