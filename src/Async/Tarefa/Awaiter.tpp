#include "Awaiter.hpp"
#include <iostream>
#include <format>

#include <Async/Escalonador.hpp>

namespace Async {

template <typename ReturnT>
template <typename FilhaReturnT>
Tarefa<ReturnT>::Awaiter<FilhaReturnT>::Awaiter(std::coroutine_handle<Promise<FilhaReturnT>> crth_filha) noexcept :
_crth_filha{crth_filha} {}



template <typename ReturnT>
template <typename FilhaReturnT>
std::coroutine_handle<> Tarefa<ReturnT>::Awaiter<FilhaReturnT>::await_suspend(std::coroutine_handle<> crth_mae) const noexcept {
  // Tentar obter uma próxima corrotina para resumir, realizando transferência simétrica.
  PromiseBase          &promise_mae = PromiseBase::fromCrth(crth_mae);
  Escalonador          *escalonador = promise_mae.escalonador();
  std::coroutine_handle<> prox_crth = escalonador->desenfileirar();
  // Caso não tenhamos obtido nenhuma corrotina, transferir para a própria subtarefa.
  if (prox_crth == std::noop_coroutine()) {
    std::cout << std::format("{} gen -> {}\n", crth_mae.address(), _crth_filha.address());
    return _crth_filha;
  }
  
  // Caso tenhamos conseguido desenfileirar, enfileirar a subtarefa e transferir para a próxima
  std::cout << std::format("{} gen {} -> {}\n", crth_mae.address(), _crth_filha.address(), prox_crth.address());
  escalonador->enfileirar(_crth_filha);
  return prox_crth;
}



template <typename ReturnT>
template <typename FilhaReturnT>
FilhaReturnT Tarefa<ReturnT>::Awaiter<FilhaReturnT>::await_resume() noexcept {
  if constexpr (Promise<FilhaReturnT>::retorna_void) {
    _crth_filha.destroy();
  } else {
    FilhaReturnT ret{_crth_filha.promise().extrairRetorno()};
    _crth_filha.destroy();
    return ret;
  }
}

}