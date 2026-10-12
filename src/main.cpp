#include <iostream>
#include <Async/Tarefa.hpp>
#include <thread>
#include <vector>
#include <format>
#include "Lifetime.hpp"
#include <format>

using namespace Async;

Tarefa<int> quantos() {
  co_return 3;
}

Tarefa<std::vector<int>> vish() {
  throw std::runtime_error("eh babado");
  co_return {1, 2, 3};
}

Tarefa<Lifetime> get_lifetime() {
  Lifetime lt;
  co_return lt;
}

Tarefa<void> stop(const std::chrono::duration<uint8_t> &t) {
  std::this_thread::sleep_for(t);
  std::cout << "interrompendo\n";

  Escalonador &esc = co_await Escalonador::Awaiter{};
  esc.interromper();
  co_return;
}

Tarefa<void> zawarudo() {
  for (int n : co_await vish())
    std::cout << std::format("za warudo array {}\n", n);
  std::cout << std::format("za warudo {}\n", co_await quantos());

  Lifetime lt = co_await get_lifetime();

  co_return;
}

int main(int argc, const char **argv) {
  namespace cr = std::chrono;

  size_t t = std::stoull(argv[1]);

  auto escalonador = Escalonador{4};

  Escalonador::Awaiter awaiter;
  Escalonador::Awaiter awaiter2{std::move(awaiter)};


  Tarefa zawarudo_task = zawarudo();
  Tarefa stop_task = stop(cr::duration<size_t>(t));

  escalonador.enfileirar(zawarudo_task);
  escalonador.enfileirar(stop_task);
  escalonador.laco_principal();

  return 0;
}