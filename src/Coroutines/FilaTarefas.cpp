#include "FilaTarefas.hpp"

#include <common.hpp>

FilaTarefas::FilaTarefas(size_t tam_inicial) {
  _buffer = sCast<Tarefa<>*>(::operator new(sizeof(Tarefa<>)));
}