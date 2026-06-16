#include "AsyncTCPWindows.hpp"

#include <iostream>
#include <system_error>

#include <common.hpp>

// --- AsyncTCP

AsyncTCP::AsyncTCP() {
  WSAData wsaData;
  if (int ret = WSAStartup(MAKEWORD(2, 2), &wsaData); ret != 0) {
    std::cerr << "AsyncTCP::AsyncTCP: Falha em 'WSAStartup()'. Código: " << std::to_string(ret)
              << ", mensagem: " << std::system_category().message(ret) << '\n';
    return;
  }
}



AsyncTCP::~AsyncTCP() {
  WSACleanup();
}