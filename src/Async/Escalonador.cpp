#include "Escalonador.hpp"
#include "Tarefa.hpp"

Escalonador::Escalonador(size_t num_workers) noexcept :
shutdown{false} {
  _workers.reserve(num_workers - 1);

  for (size_t i = 1; i <= num_workers; ++i)
    _workers.emplace_back([this, i](){
      lacoPrincipalWorker(i);
    });
}



void Escalonador::lacoPrincipalWorker(size_t i) noexcept {
  std::coroutine_handle<> crth;

  while (true) {
    {std::unique_lock lock{_fila_tarefas_mtx};
      if (shutdown) break;

      if (_fila_tarefas.empty())
        _fila_tarefas_cv.wait(lock);

      if (shutdown) break;
      crth = _fila_tarefas.front();
      _fila_tarefas.pop();
    }

    if (crth.done()) crth.destroy();
    else crth.resume();
  }
}



void Escalonador::enfileirar(std::coroutine_handle<>) noexcept {
}



void Escalonador::enfileirar(Tarefa<> tarefa) noexcept {
}

void Escalonador::interromper() noexcept {
}

std::coroutine_handle<> Escalonador::desenfileirar() noexcept {
  return std::coroutine_handle<>();
}
