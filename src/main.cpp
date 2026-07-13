#include <iostream>
#include <Async/Tarefa.hpp>
#include <Async/Escalonador.hpp>
#include <Async/Trava.hpp>
#include <thread>
#include <vector>
#include <format>

Async::Trava t;

Async::Tarefa<int> quantos() {
  co_await t.adquirir();
  t.liberar();
  co_return 3;
}

Async::Tarefa<std::vector<int>> vish() {
  co_return {1, 2, 3};
}

Async::Tarefa<void> stop(const std::chrono::duration<uint8_t> &t, Async::Escalonador &esc) {
  std::this_thread::sleep_for(t);
  std::cout << "interrompendo\n";
  esc.interromper();
  co_return;
}

Async::Tarefa<void> zawarudo() {
  for (int n : co_await vish()) {
    std::cout << std::format("za warudo array {}\n", n);  
  }
  std::cout << std::format("za warudo {}\n", co_await quantos());

  co_return;
}

int main(int argc, char **argv) {
  namespace cr = std::chrono;

  size_t t = std::stoull(argv[1]);

  auto escalonador = Async::Escalonador{4};

  Async::Tarefa zawarudo_task = zawarudo();
  Async::Tarefa stop_task = stop(cr::duration<size_t>(t), escalonador);

  escalonador.enfileirar(zawarudo_task);
  escalonador.enfileirar(stop_task);
  escalonador.lacoPrincipal();

  return 0;
}