#include "Awaiter.hpp"
#include <iostream>

#include <Async/Escalonador.hpp>

namespace Async {

template <typename ReturnT>
template <typename FilhaReturnT>
Tarefa<ReturnT>::Awaiter<FilhaReturnT>::Awaiter(std::coroutine_handle<Promise<FilhaReturnT>> crth_filha) noexcept :
_crth_filha{crth_filha} {}



template <typename ReturnT>
template <typename FilhaReturnT>
std::coroutine_handle<PromiseBase> Tarefa<ReturnT>::Awaiter<FilhaReturnT>::await_suspend(std::coroutine_handle<Promise<ReturnT>> crth) const noexcept {
  // Tentar obter uma próxima co-rotina para resumir, realizando transferência simétrica.
  Escalonador                       *escalonador = _crth_filha.promise().escalonador();
  std::coroutine_handle<PromiseBase> prox_crth   = escalonador->desenfileirar();
  // Caso não tenhamos obtido nenhuma co-rotina, transferir para a própria subtarefa.
  if (prox_crth == std::noop_coroutine()) {
    return _crth_filha.promise().crth();
  }

  // Caso tenhamos conseguido desenfileirar, enfileirar a subtarefa e transferir para a próxima
  escalonador->enfileirar(_crth_filha.promise().crth());
  return prox_crth;
}



template <typename ReturnT>
template <typename FilhaReturnT>
FilhaReturnT Tarefa<ReturnT>::Awaiter<FilhaReturnT>::await_resume() {
  if constexpr (Promise<FilhaReturnT>::retorna_void) {
    _crth_filha.destroy();
  } else {
    auto ret = _crth_filha.promise().extrairValorRetorno();
    _crth_filha.destroy();
    return *ret;
  }
}

}