#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <optional>
#include <concepts>

#include "Tarefa.hpp"

class Escalonador {

private:

  std::vector<std::thread> _workers;
  std::queue<std::coroutine_handle<>> _fila_tarefas;
  std::condition_variable _fila_tarefas_cv;
  std::mutex _fila_tarefas_mtx;
  
public:
  Escalonador(size_t num_workers) noexcept;

  ~Escalonador() noexcept = default;

private:
  void lacoPrincipalWorker(size_t i = 0) noexcept;

public:
  void enfileirar(std::coroutine_handle<>);
  void enfileirar(Tarefa<void> tarefa);
  void interromper();
  inline void lacoPrincipal() noexcept { lacoPrincipalWorker(); }
  std::coroutine_handle<> desenfileirar();
};