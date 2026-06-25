#include "Promise.hpp"

// 1st-party
#include <Async/Escalonador.hpp>
#include "AwaiterTarefa.hpp"
#include "AwaiterFinal.hpp"

template <typename TarefaReturnT>
template <typename PromiseDerivadaT>
constexpr Tarefa<TarefaReturnT>::PromiseBase<PromiseDerivadaT>::PromiseBase() noexcept :
_crth{std::coroutine_handle<PromiseDerivadaT>::from_promise(*sCast<PromiseDerivadaT *>(this))},
_crth_mae{nullptr},
_escalonador{nullptr},
_excecao{} {}



template <typename TarefaReturnT>
template <typename PromiseDerivadaT>
Tarefa<TarefaReturnT> Tarefa<TarefaReturnT>::PromiseBase<PromiseDerivadaT>::get_return_object() {
  return Tarefa<TarefaReturnT>{_crth};
}



template <typename TarefaReturnT>
template <typename PromiseDerivadaT>
Tarefa<TarefaReturnT>::AwaiterFinal Tarefa<TarefaReturnT>::PromiseBase<PromiseDerivadaT>::final_suspend() const noexcept {
  return {_crth_mae, _escalonador};
}


template <typename TarefaReturnT>
template <typename PromiseDerivadaT>
template <typename SubTarefaReturnT>
Tarefa<TarefaReturnT>::AwaiterTarefa<SubTarefaReturnT> Tarefa<TarefaReturnT>::PromiseBase<PromiseDerivadaT>::await_transform(Tarefa<SubTarefaReturnT> subtarefa) {
  return {subtarefa._crth, _escalonador};
}



// Tarefa<TarefaReturnT>::Promise

template <typename TarefaReturnT>
constexpr Tarefa<TarefaReturnT>::Promise::Promise() noexcept :
_ret{} {}