#include "Escalonador.hpp"

// std
#include <iostream>

// 1st-party
#include "Tarefa.hpp"

namespace Async {

Escalonador::Escalonador(size_t num_workers) noexcept :
_shutdown{false} {
  _workers.reserve(num_workers - 1);

  for (size_t i = 1; i < num_workers; ++i)
    _workers.emplace_back([this, i](){
      lacoPrincipalWorker(i);
    });
}



Escalonador::~Escalonador() noexcept {
  for (std::thread &worker : _workers) worker.join();
}



void Escalonador::lacoPrincipalWorker(size_t i) noexcept {
  std::cout << std::format("Thread {} elencada no escalonador\n", i);

  std::coroutine_handle<> crth_atual{nullptr};

  while (true) {
    {std::unique_lock lock{_fila_tarefas_mtx};
      _fila_tarefas_cv.wait(lock, [this] {
        return !_fila_tarefas.empty() || _shutdown;
      });

      if (_shutdown) break;

      crth_atual = _fila_tarefas.front();
      _fila_tarefas.pop();
    }

    if (crth_atual.done()) crth_atual.destroy();
    else crth_atual.resume();
  }

  std::cout << std::format("Thread {} finalizada\n", i);
}



void Escalonador::enfileirar(std::coroutine_handle<> crth) noexcept {
  {std::lock_guard lock(_fila_tarefas_mtx);
    _fila_tarefas.push(crth);
    _fila_tarefas_cv.notify_one();
  }
}



void Escalonador::enfileirar(Tarefa<void> tarefa) noexcept {
  tarefa._crth.promise()._escalonador = this;
  enfileirar(tarefa._crth);
}



void Escalonador::interromper() noexcept {
  {std::lock_guard lock{_fila_tarefas_mtx};
    _shutdown = true;
    _fila_tarefas_cv.notify_all();
  }
}



void Escalonador::abortarPendentes() noexcept {

}



std::coroutine_handle<> Escalonador::desenfileirar() noexcept {
  std::coroutine_handle<> crth{std::noop_coroutine()};

  {std::lock_guard lock{_fila_tarefas_mtx};
    if (!_shutdown && !_fila_tarefas.empty()) {
      crth = _fila_tarefas.front();
      _fila_tarefas.pop();
    }
  }

  return crth;
}

}