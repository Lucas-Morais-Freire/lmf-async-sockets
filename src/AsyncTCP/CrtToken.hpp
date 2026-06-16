#pragma once

#include <atomic>
#include <coroutine>

class AwaiterToken {
private:
  std::atomic<bool> _escalonavel{false};
  std::coroutine_handle<> _crth{nullptr};
  void *_ctx;

public:
  AwaiterToken(void *ctx);
  void setResumivel(std::coroutine_handle<> crth);
  void setFinalizada();
  inline void *getCtx() { return _ctx; }
};