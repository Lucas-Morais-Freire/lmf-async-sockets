#include "Escalonador.hpp"

Escalonador::Escalonador(size_t num_workers) noexcept {
  _workers.reserve(num_workers - 1);

  for (size_t i = 1; i <= num_workers; ++i)
    _workers.emplace_back([i](){
      lacoPrincipalWorker(i);
    });
}



void Escalonador::lacoPrincipal() {
  lacoPrincipalWorker(0);
}



void Escalonador::lacoPrincipalWorker(size_t i) {
  
}



void Escalonador::enfileirar(std::coroutine_handle<>) {
}



void Escalonador::enfileirar(Tarefa<void> tarefa) {
}

void Escalonador::interromper() {
}
