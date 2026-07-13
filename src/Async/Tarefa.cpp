#include "Tarefa.hpp"

namespace Async {

PromiseBase::PromiseBase() noexcept :
_crth_mae{nullptr},
_escalonador{nullptr},
_excecao{nullptr} {}

}