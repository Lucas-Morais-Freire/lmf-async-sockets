#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

#include "Tarefa.hpp"

class Escalonador {

private:

  std::vector<std::thread> _thread_pool;
  std::vector<std::condition_variable> _cvs;
  std::vector<std::mutex> _mtxs;
  std::queue<> _fila_tarefas;

public:
  void adicionar();
};