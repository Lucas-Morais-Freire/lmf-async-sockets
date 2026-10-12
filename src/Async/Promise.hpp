#pragma once

#include "Escalonador.hpp"

namespace Async {

class PromiseBase {
private:
  std::coroutine_handle<PromiseBase> _crth_mae;
  Escalonador *_escalonador;
  std::exception_ptr _excecao;

public:
  PromiseBase() noexcept :
  _crth_mae{nullptr}, _escalonador{nullptr}, _excecao{nullptr} {}
  COLETOR_DELETE_MOVE_COPY(PromiseBase)
  ~PromiseBase() noexcept = default;

protected:
  template <typename ReturnT>
  Tarefa<ReturnT> get_return_object() noexcept;
  template <typename ReturnT, typename FilhaReturnT>
  typename Tarefa<ReturnT>::Awaiter<FilhaReturnT> await_transform(Tarefa<FilhaReturnT> tarefa_filha) noexcept;
  template <typename ReturnT>
  typename Tarefa<ReturnT>::AwaiterFinal final_suspend() const noexcept;

public:
  inline static PromiseBase &from_crth(std::coroutine_handle<> crth) { return std::coroutine_handle<PromiseBase>::from_address(crth.address()).promise(); }
  inline std::suspend_always initial_suspend() const noexcept { return {}; }
  inline void unhandled_exception() { _excecao = std::current_exception(); }
  template <typename AwaiterT>
  AwaiterT &&await_transform(AwaiterT &&awaiter) noexcept;
  Escalonador::Awaiter await_transform(Escalonador::Awaiter &&awaiter);
  inline std::coroutine_handle<PromiseBase> crth() { return std::coroutine_handle<PromiseBase>::from_promise(*this); }
  inline void escalonador(Escalonador *escalonador) noexcept { _escalonador = escalonador; }
  inline Escalonador *escalonador() { return _escalonador; }
  inline std::exception_ptr exception() { return _excecao; }
};



template <typename ReturnT>
class Promise : public PromiseBase {
private:
  std::optional<ReturnT> _ret;

public:
  Promise() noexcept : _ret{} {}
  COLETOR_DELETE_MOVE_COPY(Promise)
  ~Promise() noexcept = default;
  inline Tarefa<ReturnT> get_return_object() noexcept { return PromiseBase::get_return_object<ReturnT>(); }
  inline void return_value(ReturnT &&ret) noexcept { _ret.emplace(std::move(ret)); }
  inline void return_value(ReturnT &ret) noexcept { _ret.emplace(std::move(ret)); }
  inline typename Tarefa<ReturnT>::AwaiterFinal final_suspend() const noexcept { return PromiseBase::final_suspend<ReturnT>(); }
  using PromiseBase::await_transform;
  template <typename FilhaReturnT>
  inline typename Tarefa<ReturnT>::Awaiter<FilhaReturnT> await_transform(Tarefa<FilhaReturnT> tarefa_filha) noexcept { return PromiseBase::await_transform<ReturnT, FilhaReturnT>(tarefa_filha); }
  inline ReturnT retorno() { return std::move(*_ret); }
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

#include "Tarefa/Awaiter.tpp"
#include "Tarefa/AwaiterFinal.tpp"
#include "Escalonador.tpp"
#include "Promise.tpp"