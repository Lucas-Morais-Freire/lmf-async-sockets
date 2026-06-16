#include "Escalonavel.hpp"

Escalonavel::Escalonavel() {}



void Escalonavel::setResumivel(std::coroutine_handle<> crth) {
  _crth = crth;

  Estado expected = Estado::INICIAL;

  if (_estado.compare_exchange_strong(
    expected,
    Estado::RESUMIVEL,
    std::memory_order_release,
    std::memory_order_relaxed
  )) return;

  expected = Estado::FINALIZADO;

  if (_estado.compare_exchange_strong(
    expected,
    Estado::PRONTO,
    std::memory_order_acq_rel,
    std::memory_order_relaxed
  )) {
    crth.resume();
    return;
  }
}



void Escalonavel::setFinalizado() {
  Estado expected = Estado::INICIAL;

  if (_estado.compare_exchange_strong(
    expected,
    Estado::FINALIZADO,
    std::memory_order_release,
    std::memory_order_relaxed
  )) return;

  expected = Estado::RESUMIVEL;

  if (_estado.compare_exchange_strong(
    expected,
    Estado::PRONTO,
    std::memory_order_acq_rel,
    std::memory_order_relaxed
  )) {
    _crth.resume();
    return;
  }
}