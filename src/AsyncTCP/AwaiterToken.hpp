#pragma once

#include <atomic>
#include <coroutine>

class AwaiterToken {
private:
  std::atomic<bool> _escalonavel{false};
  std::coroutine_handle<> _crth{nullptr};

public:
  void *_ctx;

  AwaiterToken(void *ctx);
  void setResumivel(std::coroutine_handle<> crth);
  void setFinalizada();
};