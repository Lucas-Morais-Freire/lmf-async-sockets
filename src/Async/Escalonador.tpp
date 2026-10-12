#pragma once

namespace Async {

template <typename ReturnT>
void Escalonador::enfileirar(Tarefa<ReturnT> tarefa) noexcept {
  PromiseBase::from_crth(tarefa._crth).escalonador(this);

  enfileirar(tarefa._crth);
}

}