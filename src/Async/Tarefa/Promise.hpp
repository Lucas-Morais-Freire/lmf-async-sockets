#pragma once

// std
#include <optional>

// 1st-party
#include <common.hpp>
#include <Async/Tarefa.hpp>

namespace Async {

class Escalonador;

template <typename ReturnT>
class Tarefa<ReturnT>::PromiseBase {
  friend Escalonador;
  friend class AwaiterFinal;
  template <typename AnyReturnT>
  friend class Tarefa<AnyReturnT>::PromiseBase;
  template <typename AnyReturnT>
  template <typename FilhaReturnT>
  friend class Tarefa<AnyReturnT>::Awaiter;

private:
  std::coroutine_handle<> _crth_mae;
  Escalonador *_escalonador;
  std::exception_ptr _excecao;

public:
  constexpr PromiseBase() noexcept;
  COLETOR_DELETE_MOVE_COPY(PromiseBase)
  ~PromiseBase() noexcept = default;

private:

  inline std::coroutine_handle<typename Tarefa<ReturnT>::Promise> crth() {
    return std::coroutine_handle<typename Tarefa<ReturnT>::Promise>::from_promise(*sCast<Tarefa<ReturnT>::Promise *>(this));
  }

public:
  Tarefa<ReturnT> get_return_object();
  inline std::suspend_always initial_suspend() const noexcept { return {}; }
  Tarefa<ReturnT>::AwaiterFinal final_suspend() const noexcept;
  inline void unhandled_exception() { _excecao = std::current_exception(); }
  template <typename AwaiterT>
  AwaiterT await_transform(AwaiterT &&awaiter);
  template <typename FilhaReturnT>
  Tarefa<ReturnT>::Awaiter<FilhaReturnT> await_transform(Tarefa<FilhaReturnT> tarefa_filha);
  inline void setEscalonador(Escalonador *escalonador) { _escalonador = escalonador; }
};



template <typename ReturnT>
class Tarefa<ReturnT>::Promise : public Tarefa<ReturnT>::PromiseBase {
private:
  std::optional<ReturnT> _ret;

public:
  inline constexpr Promise() noexcept : _ret{} {}
  COLETOR_DELETE_MOVE_COPY(Promise)
  ~Promise() noexcept = default;
  inline void return_value(const ReturnT &ret) noexcept { _ret.emplace(std::move(ret)); }
  inline std::optional<ReturnT> extrairValorRetorno() { return std::move(_ret); }
  static constexpr bool retorna_void = false;
};



template <>
class Tarefa<void>::Promise : public Tarefa<void>::PromiseBase {
public:
  constexpr Promise() noexcept = default;
  ~Promise() noexcept = default;
  COLETOR_DELETE_MOVE_COPY(Promise)
  inline void return_void() const noexcept {};

  static constexpr bool retorna_void = true;
};

}

#include "Promise.tpp"
