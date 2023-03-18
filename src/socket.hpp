#pragma once
#ifdef _WIN32
#include <Ws2tcpip.h>
#include <winsock2.h>
typedef unsigned int SOCKET;
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
typedef int SOCKET ;
#define INVALID_SOCKET -1
#endif

#include <errno.h>
#include <iostream>
#include <string.h>

#define LOCAL_IP "127.0.0.1"

using namespace std;

class IPAdderss {
public:
  IPAdderss() = default;
  IPAdderss(string string_address, int port);
  sockaddr_in addr;
};

class Socket {
private:
  SOCKET handle = INVALID_SOCKET;
  int port;
  void DisposeSocket();
public:
  Socket() = default;
  Socket(SOCKET sock);
  Socket(Socket& s) = delete;
  Socket(Socket&& s);
  Socket& operator=(Socket& s) = delete;
  Socket& operator=(Socket&& s);
  ~Socket();

  void Open();
  void Connect(IPAdderss client_address);
  void Bind(int port);
  Socket Accept();

  void Send(const void* buffer, int size);
  void Receive(void* buffer, int size);
};
