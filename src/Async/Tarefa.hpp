#pragma once

#include <coroutine>
#include <exception>
#include <optional>

// 1st-party
#include <common.hpp>

namespace Async {

template <typename ReturnT>
class Promise;


/** @brief Wrapper para Promise<ReturnT> */
template <typename ReturnT = void>
class Tarefa {
  friend class Escalonador;
  friend class PromiseBase;

public:
  template <typename FilhaReturnT>
  class Awaiter;
  class AwaiterFinal;
  using promise_type = Promise<ReturnT>;

  std::coroutine_handle<Promise<ReturnT>> _crth;

  inline explicit Tarefa(std::coroutine_handle<Promise<ReturnT>> crth) : _crth{crth} {}
};



class Escalonador;

class PromiseBase {
private:
  std::coroutine_handle<PromiseBase> _crth_mae;
  Escalonador *_escalonador;
  std::exception_ptr _excecao;

public:
  PromiseBase() noexcept;
  COLETOR_DELETE_MOVE_COPY(PromiseBase)
  ~PromiseBase() noexcept = default;

protected:
  template <typename ReturnT>
  typename Tarefa<ReturnT>::AwaiterFinal final_suspend() const noexcept;
  template <typename ReturnT>
  Tarefa<ReturnT> get_return_object() noexcept;
  template <typename ReturnT, typename FilhaReturnT>
  typename Tarefa<ReturnT>::Awaiter<FilhaReturnT> await_transform(Tarefa<FilhaReturnT> tarefa_filha) noexcept;

public:
  inline std::suspend_always initial_suspend() const noexcept { return {}; }
  inline void unhandled_exception() { _excecao = std::current_exception(); }
  template <typename AwaiterT>
  AwaiterT await_transform(AwaiterT &&awaiter) noexcept;
  inline std::coroutine_handle<PromiseBase> crth() { return std::coroutine_handle<PromiseBase>::from_address(this); }
  inline void setEscalonador(Escalonador *escalonador) noexcept { _escalonador = escalonador; }
  inline Escalonador *escalonador() { return _escalonador; }
};

}

#include "Tarefa/AwaiterFinal.hpp"
#include "Tarefa/Awaiter.hpp"

namespace Async {

template <typename ReturnT>
class Promise : public PromiseBase {
private:
  std::optional<ReturnT> _ret;

public:
  Promise() noexcept : _ret{} {}
  COLETOR_DELETE_MOVE_COPY(Promise)
  ~Promise() noexcept = default;
  inline Tarefa<ReturnT> get_return_object() noexcept { return PromiseBase::get_return_object<ReturnT>(); }
  inline void return_value(const ReturnT &ret) noexcept { _ret.emplace(std::move(ret)); }
  inline typename Tarefa<ReturnT>::AwaiterFinal final_suspend() const noexcept { return PromiseBase::final_suspend<ReturnT>(); }
  using PromiseBase::await_transform;
  template <typename FilhaReturnT>
  inline typename Tarefa<ReturnT>::Awaiter<FilhaReturnT> await_transform(Tarefa<FilhaReturnT> tarefa_filha) noexcept { return PromiseBase::await_transform<ReturnT, FilhaReturnT>(tarefa_filha); }
  inline std::optional<ReturnT> extrairValorRetorno() { return std::move(_ret); }
  static constexpr bool retorna_void = false;
};



template <>
class Promise<void> : public PromiseBase {
public:
  Promise() noexcept = default;
  COLETOR_DELETE_MOVE_COPY(Promise)
  ~Promise() noexcept = default;
  inline Tarefa<void> get_return_object() noexcept { return PromiseBase::get_return_object<void>(); }
  inline void return_void() const noexcept {};
  inline typename Tarefa<void>::AwaiterFinal final_suspend() const noexcept { return PromiseBase::final_suspend<void>(); }
  using PromiseBase::await_transform;
  template <typename FilhaReturnT>
  inline typename Tarefa<void>::Awaiter<FilhaReturnT> await_transform(Tarefa<FilhaReturnT> tarefa_filha) noexcept { return PromiseBase::await_transform<void, FilhaReturnT>(tarefa_filha); }
  static constexpr bool retorna_void = true;
};

}

#include "Tarefa.tpp"