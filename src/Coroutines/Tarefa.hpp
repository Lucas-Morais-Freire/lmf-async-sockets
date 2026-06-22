#pragma once

// std
#include <coroutine>
#include <chrono>

// 1st-party
#include "CategoriaPrioridade.hpp"

template <typename T = void>
class Tarefa {
  friend class Escalonador;

private:

public:
  class PromiseBase;
  class Promise;

protected:
  std::coroutine_handle<Promise> _crth;

public:
  Tarefa(std::coroutine_handle<Promise> crth);
};

#include "Promise.hpp"

