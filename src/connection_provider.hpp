#pragma once
#include "socket.hpp"
#include <string>

using namespace std;

class ConnectionProrived {
public:
  ConnectionProrived() = default;
  virtual Socket EstablishConnect() = 0;
};

class HostConnectionProvider : public ConnectionProrived {
private:
  int port;

public:
  HostConnectionProvider(int port);
  Socket EstablishConnect();
};

class ClientConnectionProvider : public ConnectionProrived {
private:
  IPAdderss host_ip;

public:
  ClientConnectionProvider(string ip_string, int port);
  Socket EstablishConnect();
};
