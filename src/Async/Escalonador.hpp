#pragma once

// std
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <coroutine>

// forward-decls
template <typename TarefaReturnT = void>
class Tarefa;

class Escalonador {

private:
  bool shutdown;
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
  void enfileirar(std::coroutine_handle<> crth) noexcept;

  /**
   * @brief Enfileira uma `Tarefa<void>` para execução assíncrona.
   * @param tarefa A tarefa a ser enfileirada.
   * @note Apenas tarefas que retornam `void` podem ser enfileiradas desta forma, pois
   * o caller não deve esperar nenhum retorno da mesma já que não vai esperar por ela.
   */
  void enfileirar(Tarefa<> tarefa) noexcept;
  void interromper() noexcept;
  void abortarPendentes() noexcept;
  inline void lacoPrincipal() noexcept { lacoPrincipalWorker(); }
  std::coroutine_handle<> desenfileirar() noexcept;
};