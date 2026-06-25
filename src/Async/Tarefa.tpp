#include "Tarefa.hpp"
#include "Tarefa/Promise.hpp"

template <typename TarefaReturnT>
Tarefa<TarefaReturnT>::Tarefa(std::coroutine_handle<Promise> crth) :
_crth{crth} {}