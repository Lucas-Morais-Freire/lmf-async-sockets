#pragma once

// std
#include <optional>

// 1st-party
#include <common.hpp>
#include <Async/Tarefa.hpp>

// forward-decls
class Escalonador;

template <typename TarefaReturnT>
template <typename PromiseDerivadaT>
class Tarefa<TarefaReturnT>::PromiseBase {
private:
  std::coroutine_handle<PromiseDerivadaT> _crth;
  std::coroutine_handle<> _crth_mae;
  Escalonador *_escalonador;
  std::exception_ptr _excecao;

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
class Tarefa<TarefaReturnT>::Promise : public PromiseBase<Promise> {
private:
  std::optional<TarefaReturnT> _ret;

public:
  constexpr Promise() noexcept;
  COLETOR_DELETE_MOVE_COPY(Promise)
  ~Promise() noexcept = default;
  inline void return_value(const TarefaReturnT &ret) noexcept { _ret.emplace(std::move(ret)); }
  inline std::optional<TarefaReturnT> extrairValorRetorno() { return std::move(_ret); }
  static constexpr bool retorna_void = false;
};



template <>
class Tarefa<void>::Promise : public PromiseBase<Promise> {
public:
  constexpr Promise() noexcept = default;
  ~Promise() noexcept = default;
  COLETOR_DELETE_MOVE_COPY(Promise)
  inline void return_void() const noexcept {};

  static constexpr bool retorna_void = true;
};

#include "Promise.tpp"