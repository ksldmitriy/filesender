#pragma once
#include "app_start_parameters.hpp"
#include "file_transfer.hpp"
#include "socket.hpp"
#include "time.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>

typedef chrono::high_resolution_clock::time_point time_point;
typedef chrono::high_resolution_clock::duration duration;

using namespace std;
namespace fs = filesystem;

class FileSender : public IFileTransfer {
private:
  vector<fs::path> files;

  void SendFiles();
  void SendFile(fs::path filename);
  void SendBatch(ifstream &file);
  void SendEOT();
  void SendEOF();
  void WaitBatchAcception();
  void EndSession();

public:
  FileSender(vector<fs::path> files, ConnectionProrived *connection_provider);
  void Run();
};
