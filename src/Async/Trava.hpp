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
  static constexpr Awaiter *TRAVADO_SEM_NOVOS_AWAITERS = reinterpret_cast<Awaiter *>(0x1);

  std::atomic<Awaiter *> _lista_novos_awaiters;
  Awaiter *_fila_awaiters_frente;
  Awaiter *_fila_awaiters_traseira;
public:
  explicit Trava() noexcept;
  COLETOR_DELETE_MOVE_COPY(Trava)
  Awaiter adquirir() noexcept;
  void liberar() noexcept;
};

}

#include "Trava/Awaiter.hpp"