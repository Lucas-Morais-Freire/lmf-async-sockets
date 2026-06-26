#include "Escalonador.hpp"

template <typename PromiseT>
void Escalonador::enfileirar(std::coroutine_handle<PromiseT> crth) noexcept {
  PromiseT &promise = crth.promise();
  promise.setEscalonador(this);
  {std::lock_guard lock{_fila_tarefas_mtx};
    _fila_tarefas.push(crth);
    _fila_tarefas_cv.notify_one();
  }
}