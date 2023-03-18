#pragma once
#include <iostream>
#include <memory>
#include <string.h>
#include <string>

using namespace std;

class IException {
protected:
  string details;

  shared_ptr<IException> sub_exception;
  virtual string GetMessage() = 0;
  void Print(int level);

public:
  IException(string details);
  void Print();
};

class SocketException : public IException {
private:
  int error;
  string GetMessage();

public:
  SocketException(string details, int error);
};

class FileException : public IException {
private:
  string GetMessage();

public:
  FileException(string details) : IException(details) {}
};

class ArgumentsException : public IException {
private:
  string GetMessage();

public:
  ArgumentsException(string details) : IException(details) {}
};
