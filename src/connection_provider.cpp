#include "connection_provider.hpp"

HostConnectionProvider::HostConnectionProvider(int port) { this->port = port; }

Socket HostConnectionProvider::EstablishConnect() {
  Socket server_sock;

  cout << "host" << endl;

  server_sock.Open();
  server_sock.Bind(port);
  Socket client_sock = server_sock.Accept();

  return client_sock;
}

ClientConnectionProvider::ClientConnectionProvider(string ip_string, int port) {
  host_ip = IPAdderss(ip_string, port);
}

Socket ClientConnectionProvider::EstablishConnect() {
  cout << "client" << endl;

  Socket sock;
  sock.Open();
  sock.Connect(host_ip);

  return sock;
}
