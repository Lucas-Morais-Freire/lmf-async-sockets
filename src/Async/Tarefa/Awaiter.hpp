#pragma once

#include <Async/Tarefa.hpp>

namespace Async {

template <typename ReturnT>
template <typename FilhaReturnT>
class Tarefa<ReturnT>::Awaiter {
private:
  std::coroutine_handle<Promise<FilhaReturnT>> _crth_filha;

public:
explicit Awaiter(std::coroutine_handle<Promise<FilhaReturnT>> crth_filha) noexcept;
  COLETOR_DELETE_MOVE_COPY(Awaiter)
  ~Awaiter() noexcept = default;
  inline bool await_ready() const noexcept { return false; }
  std::coroutine_handle<> await_suspend(std::coroutine_handle<> crth_mae) const noexcept;
  FilhaReturnT await_resume();
};

}

#include "AwaiterFinal.hpp"