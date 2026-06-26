#include "AwaiterTarefa.hpp"
#include <iostream>

template <typename ReturnT>
template <typename FilhaReturnT>
Tarefa<ReturnT>::AwaiterTarefa<FilhaReturnT>::AwaiterTarefa(std::coroutine_handle<typename Tarefa<FilhaReturnT>::Promise> crth_filha) noexcept :
_crth_filha{crth_filha} {}



template <typename ReturnT>
template <typename FilhaReturnT>
std::coroutine_handle<> Tarefa<ReturnT>::AwaiterTarefa<FilhaReturnT>::await_suspend(std::coroutine_handle<> crth) const noexcept {
  // Tentar obter uma próxima co-rotina para resumir, realizando transferência simétrica.
  Escalonador            *escalonador = _crth_filha.promise()._escalonador;
  std::coroutine_handle<> prox_crth   = escalonador->desenfileirar();
  // Caso não tenhamos obtido nenhuma co-rotina, transferir para a própria subtarefa.
  if (!prox_crth) {
    return _crth_filha;
  }

  // Caso tenhamos conseguido desenfileirar, enfileirar a subtarefa e transferir para a próxima
  escalonador->enfileirar(_crth_filha);
  return prox_crth;
}



template <typename ReturnT>
template <typename FilhaReturnT>
FilhaReturnT Tarefa<ReturnT>::AwaiterTarefa<FilhaReturnT>::await_resume() {
  if constexpr (Tarefa<FilhaReturnT>::Promise::retorna_void) {
    _crth_filha.destroy();
  } else {
    auto ret = _crth_filha.promise().extrairValorRetorno();
    _crth_filha.destroy();
    return *ret;
  }
}