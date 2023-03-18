#include "exception.hpp"

// Exception

IException::IException(string details) { this->details = details; }

void IException::Print() { Print(0); }

void IException::Print(int level) {
  cout << string('\t', level) << GetMessage() << endl;

  if (sub_exception) {
    sub_exception->Print(level + 1);
  }
}

// ScoketException

SocketException::SocketException(string details, int error)
    : IException(details) {
  this->error = error;
}

string SocketException::GetMessage() {
  string message;
  message += "socket error: ";
  message += details + " ";
  message += strerror(errno);
  return message;
}

// FileException



// ArgumentsException

string ArgumentsException::GetMessage(){
  string message;
  message += "invalid arguments: ";
  message += details;
  return message;
};

