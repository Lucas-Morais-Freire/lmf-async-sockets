#pragma once

#include "Tarefa.hpp"
#include <atomic>

class FilaTarefas {
private:
  Tarefa<> *_buffer;
  std::atomic<size_t> _frente;
  std::atomic<size_t> _traseira;

public:
  FilaTarefas(size_t tam_inicial);
};