#include "Promise.hpp"

// 1st-party
#include <Async/Escalonador.hpp>
#include "AwaiterTarefa.hpp"
#include "AwaiterFinal.hpp"

template <typename ReturnT>
constexpr Tarefa<ReturnT>::PromiseBase::PromiseBase() noexcept :
_crth_mae{nullptr},
_escalonador{nullptr},
_excecao{} {}



template <typename ReturnT>
Tarefa<ReturnT> Tarefa<ReturnT>::PromiseBase::get_return_object() {
  return Tarefa<ReturnT>{crth()};
}



template <typename ReturnT>
Tarefa<ReturnT>::AwaiterFinal Tarefa<ReturnT>::PromiseBase::final_suspend() const noexcept {
  return AwaiterFinal{_crth_mae};
}


template <typename ReturnT>
template <typename FilhaReturnT>
Tarefa<ReturnT>::AwaiterTarefa<FilhaReturnT> Tarefa<ReturnT>::PromiseBase::await_transform(Tarefa<FilhaReturnT> tarefa_filha) {
  tarefa_filha._crth.promise()._escalonador = _escalonador;
  tarefa_filha._crth.promise()._crth_mae = crth();
  return AwaiterTarefa<FilhaReturnT>{tarefa_filha._crth};
}