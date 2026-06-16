#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#include <string>
#include <vector>
#include <coroutine>
#include <cstdint>

#include "EnderecoIP.hpp"

class AsyncTCP {
public:
  class DNSSolve;
  class Connect;
  class Recv;
  class Send;

private:
  SOCKET _socket = INVALID_SOCKET;

  int _last_operation_ret = SOCKET_ERROR;

public:
  AsyncTCP();
  virtual ~AsyncTCP();
};

#include "DNSSolveWindows.hpp"