#pragma once

#include <Async/Trava.hpp>
#include <common.hpp>

namespace Async {

class Escalonador;

class Trava::Awaiter {
  friend Trava;

private:
  Trava &_trava;
  Awaiter *_next;
  Escalonador *_escalonador;
  std::coroutine_handle<> _crth;

public:
  Awaiter(Trava &trava) noexcept;
  Awaiter(Awaiter &&other) noexcept;
  Awaiter &operator=(Awaiter &&other) noexcept = delete;
  COLETOR_DELETE_COPY(Awaiter)
  ~Awaiter() noexcept = default;
  inline void setContexto(Escalonador *escalonador, std::coroutine_handle<> crth) {
    _escalonador = escalonador;
    _crth = crth;
  }
  bool await_ready() noexcept;
  std::coroutine_handle<> await_suspend(std::coroutine_handle<> crth) noexcept;
  inline void await_resume() const noexcept {}
};

}