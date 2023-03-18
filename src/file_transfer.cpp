#include "file_transfer.hpp"
#include "socket.hpp"

using namespace std;

IFileTransfer::IFileTransfer(ConnectionProrived* connection_provider){
  sock = connection_provider->EstablishConnect();
}

void IFileTransfer::SendInt(int n) {
  n = htonl(n);
  sock.Send(&n, sizeof(n));
}

int IFileTransfer::ReceiveInt() {
  int n;
  sock.Receive(&n, sizeof(n));
  n = ntohl(n);
  return n;
}

void IFileTransfer::SendString(string str) {
  SendInt(str.size());

  sock.Send(str.c_str(), str.size());
}

string IFileTransfer::ReceiveString() {
  int str_size = ReceiveInt();

  vector<char> buffer(str_size);
  sock.Receive(buffer.data(), str_size);
  string str(buffer.data(), str_size);

  return str;
}

void IFileTransfer::SendMessage(MessageHeader header) {
  header.type = header.type;
  header.size = htonl(header.size);

  sock.Send(&header, sizeof(header));
}

MessageHeader IFileTransfer::ReceiveMessage() {
  MessageHeader header;
  sock.Receive(&header, sizeof(header));

  header.size = ntohl(header.size);

  return header;
}

void IFileTransfer::SendBuffer(void *buffer, int size) {
  MessageHeader header;
  header.type = MessageHeaderType::data_buffer;
  header.size = size;

  SendMessage(header);
  sock.Send(buffer, size);
}

void IFileTransfer::ReceiveBuffer(void *buffer, int size) {
  sock.Receive(buffer, size);
}
