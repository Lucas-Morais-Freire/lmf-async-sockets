#pragma once

// std
#include <coroutine>

template <typename T>
class Task {
public:
  class Promise {
    std::suspend_always initial_suspend();
    std::suspend_always final_suspend();
  };

private:
  std::coroutine_handle<Promise> _crth;
};