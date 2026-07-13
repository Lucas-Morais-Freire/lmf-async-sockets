#pragma once

#include <Async/Trava.hpp>

// std
#include <atomic>
#include <coroutine>

// 1st-party
#include <common.hpp>

namespace Async {

class Escalonador;

class PromiseBase;

/** @brief Awaiter responsável por gerenciar suspensão e retomada de corrotinas que adquirem travas. */
class Trava::Awaiter {
  friend Trava;

private:
  /** @brief Referência à trava que este awaiter está esperando para adquirir. */
  Trava &_trava;
  /** @brief Ponteiro para o próximo awaiter na lista de awaiters esperando para adquirir `_trava`. */
  Awaiter *_next;
  /** @brief Corrotina que será resumida quando este awaiter conseguir adquirir a trava. */
  std::coroutine_handle<PromiseBase> _crth;

public:
  /** @brief Construtor padrão. */
  Awaiter(Trava &trava) noexcept;
  /** @brief Construtor de movimento. */
  Awaiter(Awaiter &&other) noexcept;
  Awaiter &operator=(Awaiter &&other) noexcept = delete;
  COLETOR_DELETE_COPY(Awaiter)
  /** @brief Destrutor */
  ~Awaiter() noexcept = default;
  bool await_ready() noexcept;
  std::coroutine_handle<PromiseBase> await_suspend(std::coroutine_handle<PromiseBase> crth) noexcept;
  inline void await_resume() const noexcept {}
};

}