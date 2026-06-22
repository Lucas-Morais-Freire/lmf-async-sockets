#pragma once

#include <optional>

#include "Tarefa.hpp"
#include "Escalonador.hpp"

template <typename T>
class Tarefa<T>::PromiseBase {
private:
  Escalonador *_escalonador;

public:
  // Métodos obrigatórios para uma co-rotina

  Tarefa<T> get_return_object() {
    return Tarefa<T>{std::coroutine_handle<Promise>::from_promise(*this)};
  }
  std::suspend_always initial_suspend() const noexcept { return {}; }
  std::suspend_always final_suspend() const noexcept { return {}; }
  // void unhandled_exception();
  // yield_value(T);
  // await_transform(T);
};



template <typename T>
class Tarefa<T>::Promise : public PromiseBase {
private:
  std::optional<T> ret;

public:
  inline void return_value(T &&retorno) noexcept {
    ret.emplace(std::move(retorno));
  }
};



template <>
class Tarefa<void>::Promise : public PromiseBase {
public:
  inline void return_void() const noexcept {};
};