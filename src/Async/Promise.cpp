#include "Promise.hpp"

namespace Async {

Escalonador::Awaiter PromiseBase::await_transform(Escalonador::Awaiter &&awaiter) {
  awaiter._esc = crth().promise().escalonador();
  return std::move(awaiter);
}

}