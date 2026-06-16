#pragma once

#include <atomic>
#include <coroutine>

class Escalonavel {
private:
  enum Estado {
    INICIAL,
    RESUMIVEL,
    FINALIZADO,
    PRONTO
  };

  std::atomic<Estado> _estado{Estado::INICIAL};
  std::coroutine_handle<> _crth{};

public:
  void setResumivel(std::coroutine_handle<> crth);
  void setFinalizado();
};