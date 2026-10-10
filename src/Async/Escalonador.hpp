#pragma once

#ifndef ASYNC_H
#error "Você deve incluir este arquivo via \"Async/Async.hpp\""
#endif

// std
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <coroutine>
#include <atomic>

// 1st-party
#include <common.hpp>

namespace Async {

// forward-decls
template <typename ReturnT>
class Tarefa;

class Escalonador {
public:
  class Awaiter;

private:
  bool _finalizar;
  std::vector<std::thread> _workers;
  std::queue<std::coroutine_handle<>> _fila_tarefas;
  std::condition_variable _fila_tarefas_cv;
  std::mutex _fila_tarefas_mtx;
  
public:
  Escalonador(size_t num_workers) noexcept;

  ~Escalonador() noexcept;

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
  template <typename ReturnT>
  void enfileirar(Tarefa<ReturnT> tarefa) noexcept;
  void interromper() noexcept;
  void abortarPendentes() noexcept;
  inline void lacoPrincipal() noexcept { lacoPrincipalWorker(); }
  std::coroutine_handle<> desenfileirar() noexcept;
};



class Escalonador::Awaiter {
  friend class PromiseBase;

private:
  Escalonador *_esc;

public:
  inline explicit Awaiter() noexcept : _esc{} {}
  COLETOR_DELETE_COPY(Awaiter)
  inline Awaiter(Awaiter &&outro) noexcept : _esc{outro._esc} {}
  inline Awaiter &operator=(Awaiter &&) = delete;
  inline ~Awaiter() noexcept = default;
  inline bool await_ready() const noexcept { return true; }
  inline std::coroutine_handle<> await_suspend(std::coroutine_handle<>) const noexcept { return std::noop_coroutine(); }
  inline Escalonador &await_resume() const noexcept { return *_esc; }
};

}