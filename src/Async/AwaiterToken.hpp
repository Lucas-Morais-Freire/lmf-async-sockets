#pragma once

// std
#include <atomic>
#include <coroutine>

// 1st-party
#include <common.hpp>

namespace Async {

class AwaiterToken {
private:
  std::atomic<bool> _escalonavel{false};
  std::coroutine_handle<> _crth{nullptr};

  void *_ctx;
public:

  AwaiterToken(void *ctx);
  AwaiterToken(const AwaiterToken &) = delete;
  AwaiterToken(AwaiterToken &&) = delete;
  AwaiterToken &operator=(const AwaiterToken &) = delete;
  AwaiterToken &operator=(AwaiterToken &&) = delete;
  ~AwaiterToken();
  void setResumivel(std::coroutine_handle<> crth);
  void setFinalizado();
  template <typename T>
  inline T &ctx() { return *sCast<T *>(_ctx); }
};

}