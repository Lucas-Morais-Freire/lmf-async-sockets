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
  static constexpr Awaiter *VAZIA = nullptr;
  static constexpr Awaiter *TRAVADO_SEM_NOVOS_AWAITERS = rCast<Awaiter *>(1ULL);

  std::atomic<Awaiter *> _lista_novos_awaiters;
  Awaiter *_lista_awaiters;
public:
  explicit Trava() noexcept;
  COLETOR_DELETE_MOVE_COPY(Trava)
  Awaiter adquirir() noexcept;
  void liberar() noexcept;
};

}

#include "Trava/Awaiter.hpp"