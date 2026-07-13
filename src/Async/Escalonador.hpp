#pragma once

// std
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <coroutine>
#include <atomic>

namespace Async {

// forward-decls
template <typename ReturnT>
class Tarefa;

class PromiseBase;

class Escalonador {
private:
  bool _shutdown;
  std::vector<std::thread> _workers;
  std::queue<std::coroutine_handle<PromiseBase>> _fila_tarefas;
  std::condition_variable _fila_tarefas_cv;
  std::mutex _fila_tarefas_mtx;
  
public:
  Escalonador(size_t num_workers) noexcept;

  ~Escalonador() noexcept;

private:
  void lacoPrincipalWorker(size_t i = 0) noexcept;
  
public:
  void enfileirar(std::coroutine_handle<PromiseBase> crth) noexcept;

  /**
   * @brief Enfileira uma `Tarefa<void>` para execução assíncrona.
   * @param tarefa A tarefa a ser enfileirada.
   * @note Apenas tarefas que retornam `void` podem ser enfileiradas desta forma, pois
   * o caller não deve esperar nenhum retorno da mesma já que não vai esperar por ela.
   */
  void enfileirar(Tarefa<void> tarefa) noexcept;
  void interromper() noexcept;
  void abortarPendentes() noexcept;
  inline void lacoPrincipal() noexcept { lacoPrincipalWorker(); }
  std::coroutine_handle<PromiseBase> desenfileirar() noexcept;
};

}