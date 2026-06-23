#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <optional>

#include "Tarefa.hpp"

class Escalonador {

private:

  std::vector<std::thread> _thread_pool;
  std::vector<std::condition_variable> _cvs;
  std::vector<std::mutex> _mtxs;
  std::queue<std::coroutine_handle<>> _fila_tarefas;

public:
  void enfileirar(std::coroutine_handle<> crth);
  std::optional<std::coroutine_handle<>> desenfileirar();
};