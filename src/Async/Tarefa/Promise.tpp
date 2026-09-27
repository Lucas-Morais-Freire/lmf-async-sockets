#include "Promise.hpp"

namespace Async {

template <typename ReturnT>
Tarefa<ReturnT> PromiseBase::get_return_object() noexcept {
  return Tarefa<ReturnT>{std::coroutine_handle<Promise<ReturnT>>::from_address(crth().address())};
}



template <typename ReturnT, typename FilhaReturnT>
typename Tarefa<ReturnT>::Awaiter<FilhaReturnT> PromiseBase::await_transform(Tarefa<FilhaReturnT> tarefa_filha) noexcept {
  PromiseBase &promise_filha = PromiseBase::fromCrth(tarefa_filha._crth);
  promise_filha._escalonador = _escalonador;
  promise_filha._crth_mae = crth();
  return typename Tarefa<ReturnT>::Awaiter<FilhaReturnT>{tarefa_filha._crth};
}



template <typename ReturnT>
typename Tarefa<ReturnT>::AwaiterFinal PromiseBase::final_suspend() const noexcept {
  return typename Tarefa<ReturnT>::AwaiterFinal{_crth_mae};
}



template <typename AwaiterT>
inline AwaiterT &&PromiseBase::await_transform(AwaiterT &&awaiter) noexcept {
  awaiter.setCrth(crth());
  return std::forward<AwaiterT>(awaiter);
}

}