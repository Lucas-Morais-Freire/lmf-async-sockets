#include <iostream>
#include <Async/Tarefa.hpp>
#include <Async/Escalonador.hpp>

Tarefa<void> teste() {
  co_return;
}

int main(int, char **) {

  auto escalonador = Escalonador{4};

  Tarefa task = teste();

  escalonador.enfileirar(task);

  escalonador.lacoPrincipal();

  return 0;
}