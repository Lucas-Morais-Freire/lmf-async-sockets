#include "Escalonador.hpp"

// std
#include <iostream>
#include <format>

namespace Async {

Escalonador::Escalonador(size_t num_workers) noexcept :
_finalizar{false} {
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
  std::coroutine_handle<> crth_atual{nullptr};

  while (true) {
    {std::unique_lock lock{_fila_tarefas_mtx};
      _fila_tarefas_cv.wait(lock, [this] {
        return !_fila_tarefas.empty() || _finalizar;
      });

      if (_finalizar) break;

      crth_atual = _fila_tarefas.front();
      _fila_tarefas.pop();
    }

    if (crth_atual.done()) {
      if (
        auto exception_ptr = PromiseBase::from_crth(crth_atual).exception();
        exception_ptr != nullptr
      ) {
        try {
          std::rethrow_exception(exception_ptr);
        } catch (const std::exception &e) {
          std::cout << std::format("Corrotina {} finalizou com exceção {}\n", crth_atual.address(), e.what());
        }
      }
      crth_atual.destroy();
    }
    else crth_atual.resume();
  }

  std::cout << std::format("Thread {} finalizada\n", i);
}



void Escalonador::enfileirar(std::coroutine_handle<> crth) noexcept {
  std::cout << std::format("enfileirando {}\n", crth.address());
  {std::lock_guard lock(_fila_tarefas_mtx);
    _fila_tarefas.push(crth);
    _fila_tarefas_cv.notify_one();
  }
}



void Escalonador::interromper() noexcept {
  {std::lock_guard lock{_fila_tarefas_mtx};
    _finalizar = true;
    _fila_tarefas_cv.notify_all();
  }
}



void Escalonador::abortarPendentes() noexcept {

}



std::coroutine_handle<> Escalonador::desenfileirar() noexcept {
  std::coroutine_handle<> crth{
    std::coroutine_handle<>::from_address(std::noop_coroutine().address())
  };

  {std::lock_guard lock{_fila_tarefas_mtx};
    if (!_finalizar && !_fila_tarefas.empty()) {
      crth = _fila_tarefas.front();
      _fila_tarefas.pop();
    }
  }

  return crth;
}

}