#pragma once

#include <optional>

#include "Tarefa.hpp"
#include "Escalonador.hpp"
#include "AwaiterTarefa.hpp"
#include "AwaiterFinal.hpp"
#include <common.hpp>

template <typename TarefaReturnT>
template <typename PromiseDerivadaT>
class Tarefa<TarefaReturnT>::PromiseBase {
private:
  std::coroutine_handle<PromiseDerivadaT> _crth;
  std::coroutine_handle<> _crth_mae;
  Escalonador *_escalonador;
  std::exception _excecao;

public:
  constexpr PromiseBase() noexcept;
  COLETOR_DELETE_MOVE_COPY(PromiseBase)
  ~PromiseBase() noexcept = default;
  Tarefa<TarefaReturnT> get_return_object();
  inline std::suspend_always initial_suspend() const noexcept { return {}; }
  AwaiterFinal final_suspend() const noexcept;
  inline void unhandled_exception() { _excecao = std::current_exception(); }
  template <typename SubTarefaReturnT>
  AwaiterTarefa<SubTarefaReturnT> await_transform(Tarefa<SubTarefaReturnT> subtarefa);
};



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
  return {_crth};
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



template <typename TarefaReturnT>
class Tarefa<TarefaReturnT>::Promise : public PromiseBase<Promise> {
private:
  std::optional<TarefaReturnT> _ret;

public:
  constexpr Promise() noexcept;
  COLETOR_DELETE_MOVE_COPY(Promise)
  ~Promise() noexcept = default;
  inline void return_value(TarefaReturnT &&ret) noexcept { _ret.emplace(std::move(ret)); }
  inline std::optional<TarefaReturnT> extrairValorRetorno() { return std::move(_ret); }
  static constexpr bool retorna_void = false;
};



template <typename TarefaReturnT>
constexpr Tarefa<TarefaReturnT>::Promise::Promise() noexcept :
_ret{} {}



template <>
class Tarefa<void>::Promise : public PromiseBase<Promise> {
public:
  constexpr Promise() noexcept = default;
  ~Promise() noexcept = default;
  COLETOR_DELETE_MOVE_COPY(Promise)
  inline void return_void() const noexcept {};

  static constexpr bool retorna_void = true;
};