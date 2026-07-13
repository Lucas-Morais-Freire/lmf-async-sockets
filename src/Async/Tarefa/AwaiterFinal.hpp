#pragma once

#include <Async/Tarefa.hpp>
#include <common.hpp>

namespace Async {

template <typename ReturnT>
class Tarefa<ReturnT>::AwaiterFinal {
private:
  std::coroutine_handle<PromiseBase> _crth_mae;

public:
  explicit AwaiterFinal(std::coroutine_handle<PromiseBase> crth_mae) noexcept;
  COLETOR_DELETE_MOVE_COPY(AwaiterFinal)
  inline ~AwaiterFinal() noexcept = default;
  inline bool await_ready() const noexcept { return false; }
  std::coroutine_handle<PromiseBase> await_suspend(std::coroutine_handle<Promise<ReturnT>> crth) const noexcept;
  inline void await_resume() const noexcept {};
};

}