#pragma once

// std
#include <atomic>
#include <coroutine>

// 1st-party
#include <common.hpp>

namespace Async {

class Trava {
private:
  class Awaiter;

  std::atomic<Awaiter *> _fila_awaiters_cabeca;
  Awaiter *_fila_awaiters_cauda;


public:
  explicit Trava() noexcept;
  COLETOR_DELETE_MOVE_COPY(Trava)
  Awaiter &adquirir() noexcept;
  void liberar() noexcept;
};

}

#include "Trava/Awaiter.hpp"