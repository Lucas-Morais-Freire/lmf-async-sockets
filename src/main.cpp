#include <iostream>
#include <Async/Tarefa.hpp>
#include <Async/Escalonador.hpp>
#include <thread>

Tarefa<void> teste() {
  std::cout << "za warudo\n";
  co_return;
}

int main(int, char **) {

  auto escalonador = Escalonador{4};

  Tarefa task1 = teste();
  Tarefa task2 = teste();

  escalonador.enfileirar(task1);
  escalonador.enfileirar(task2);

  std::this_thread::sleep_for(std::chrono::duration<uint8_t>(1));
  escalonador.interromper();
  std::this_thread::sleep_for(std::chrono::duration<uint8_t>(1));

  return 0;
}