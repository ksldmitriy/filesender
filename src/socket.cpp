#include "socket.hpp"
#include "exception.hpp"

Socket::Socket(SOCKET sock) { handle = sock; }

Socket::Socket(Socket &&s) { *this = move(s); }

Socket &Socket::operator=(Socket &&s) {
  DisposeSocket();
  handle = s.handle;
  s.handle = INVALID_SOCKET;

  return *this;
}

Socket::~Socket() { DisposeSocket(); }

void Socket::DisposeSocket() {
  if (handle == INVALID_SOCKET) {
    return;
  }

  int status = 0;

#ifdef _WIN32
  status = shutdown(handle, SD_BOTH);
  if (status == 0) {
    status = closehandleet(handle);
  }
#else
  status = shutdown(handle, SHUT_RDWR);
  status = close(handle);
  if (status) {
    cout << "close socket error: " << strerror(errno) << endl;
  }
#endif
}

void Socket::Open() {
  handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (handle == INVALID_SOCKET) {
    SocketException e("cant create socket", errno);
    throw e;
  }
}

void Socket::Connect(IPAdderss client_address) {
  int result = connect(handle, (sockaddr *)&client_address.addr,
                       sizeof(client_address.addr));
  if (result) {
    SocketException e("cant connect", errno);
    throw e;
  }

  cout << "connected" << endl;
}

void Socket::Bind(int port) {
  IPAdderss ip("ANY", port);

  int result = bind(handle, (sockaddr *)&ip.addr, sizeof(ip.addr));
  if (result < 0) {
    SocketException e("cant bind socket", errno);
    throw e;
  }
}

Socket Socket::Accept() {
  int result = listen(handle, 1);
  if (result) {
    SocketException e("socket listen error", errno);
    throw e;
  }

  sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  SOCKET client_socket =
      accept(handle, (sockaddr *)&client_addr, &client_addr_len);
  if (client_socket == INVALID_SOCKET) {
    SocketException e("cant accept client", errno);
    throw e;
  }

  cout << "accepted" << endl;

  return Socket(client_socket);
}

void Socket::Send(const void *buffer, int size) {
  int sent = 0;
  while (sent != size) {
    int result = send(handle, (const char *)buffer + sent, size - sent, 0);
    if (result == -1) {
      SocketException e("send error", errno);
      throw e;
    }

    sent += result;
  }
}

void Socket::Receive(void *buffer, int size) {
  int received = 0;
  while (received != size) {
    int result = recv(handle, (char *)buffer + received, size - received, 0);
    if (result <= 0) {
      SocketException e("receive error", errno);
      throw e;
    }

    received += result;
  }
}

IPAdderss::IPAdderss(string string_address, int port) {
  memset(&addr, '\0', sizeof(addr));

  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);

  if (string_address == "ANY") {
    addr.sin_addr.s_addr = INADDR_ANY;
  } else {
    int result = inet_pton(AF_INET, string_address.c_str(), &addr.sin_addr);
    if (result != 1) {
      ArgumentsException e("parse ip error");
      throw e;
    }
  }
}
