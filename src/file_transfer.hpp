#pragma once
#include "app_start_parameters.hpp"
#include "connection_provider.hpp"
#include "network_sturcts.hpp"
#include "socket.hpp"
#include "time.h"
#include <string>

using namespace std;

class IFileTransfer {
protected:
  Socket sock;

  void SendInt(int n);
  int ReceiveInt();
  void SendString(string str);
  string ReceiveString();
  void SendMessage(MessageHeader header);
  MessageHeader ReceiveMessage();
  void SendBuffer(void *buffer, int size);
  void ReceiveBuffer(void *buffer, int size);

public:
  IFileTransfer(ConnectionProrived *connection_provider);
  virtual void Run() = 0;
};
