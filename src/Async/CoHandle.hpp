// #pragma once

// #include <coroutine>
// #include <common.hpp>

// namespace Async {

// class PromiseBase;

// template <typename ReturnT>
// class Promise;

// template <typename PromiseT = PromiseBase>
// class CoHandle;

// template <typename PromiseT>
// class CoHandle {
// private:
//   std::coroutine_handle<PromiseT> _crth;

// public:
//   explicit CoHandle(std::coroutine_handle<> crth) :
//   _crth{std::coroutine_handle<PromiseT>::from_address(crth.address())} {}
//   COLETOR_DEFAULT_COPY_MOVE(CoHandle)
//   ~CoHandle() noexcept = default;
//   template <typename AnyPromiseT>
//   inline std::coroutine_handle<AnyPromiseT> get() noexcept { return std::coroutine_handle<AnyPromiseT>::from_address(_crth.address()); }
//   CoHandle<PromiseBase> operator() const noexcept;
// };

// template <>
// class CoHandle<PromiseBase> {
// private:
//   std::coroutine_handle<PromiseBase> _crth;

// public:
//   inline std::coroutine_handle<PromiseBase> get() { return _crth; }
// };

// }