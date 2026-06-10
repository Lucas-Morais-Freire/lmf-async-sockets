#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <coroutine>
#include <iostream>

class AsyncSocket {
public:
  class Connect; friend Connect;
  class Recv; friend Recv;
  class Send; friend Send;

private:
  SOCKET _socket = INVALID_SOCKET;

  int _last_operation_ret = SOCKET_ERROR;

public:
  AsyncSocket();
};



class AsyncSocket::Connect {
  friend AsyncSocket;

private:
  bool result = false;

  AsyncSocket &_asocket;
  const std::string &_peer_hostname;
  const std::string &_peer_port;
  addrinfo *_addr = nullptr;

  Connect(AsyncSocket &asocket, const std::string &peer_hostname, const std::string &peer_port) :
  _asocket(asocket),
  _peer_hostname(peer_hostname),
  _peer_port(peer_port) {}


public:
  bool await_ready() {
    addrinfo hints = {};
    hints.ai_family   = AF_UNSPEC;   // Aceita IPv4 ou IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    
    // Resolver o hostname. em `_addr` será retornada uma linkedlist de IPs possíveis.
    int ret = getaddrinfo(_peer_hostname.c_str(), _peer_port.c_str(), &hints, &_addr);
    // Em caso de erro, não precisamos suspender, mas o resultado será falso.
    if (ret != 0) {
      std::cout << "AsyncSocket::Connect::await_ready: Erro na chamada de getaddrinfo. Código: "
                << std::to_string(ret) << ", mensagem: " << gai_strerror(ret) << '\n';
      return true;
    }

    // Fazer tentativa inicial
    _asocket._socket = socket(_addr->ai_family, _addr->ai_socktype, _addr->ai_protocol);
    if (_asocket._socket == INVALID_SOCKET) return true;

    return connect(_asocket._socket, _addr->ai_addr, _addr->ai_addrlen) != SOCKET_ERROR;
  }



  bool await_suspend(std::coroutine_handle<> crth) {
    addrinfo *curr_addr = _addr;

    do {
      int cod = WSAGetLastError();
      if (cod != WSAEWOULDBLOCK) {
        closesocket(_asocket._socket);
        _asocket._socket = INVALID_SOCKET;
        curr_addr = curr_addr->ai_next;
        continue;
      }


    } while (curr_addr != nullptr);
  }
};