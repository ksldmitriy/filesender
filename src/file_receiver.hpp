#pragma once
#include "file_transfer.hpp"
#include <fstream>

using namespace std;

class FileReceiver : public IFileTransfer{
private:
  enum State {
	wait_for_name,
	wait_for_data,
  };

  char buffer[send_buffer_size];
  
  State state = State::wait_for_name;
  ofstream current_file;
  string current_filename;

  void ReceiveLoop();
  bool ProcessHeader(MessageHeader header);
  bool ProccessFilenameReceive(MessageHeader header);
  void ProccessDataReceive(MessageHeader header);
  void SendBatchAcception();
public:
  FileReceiver(ConnectionProrived* connection_provider);
  void Run();
};
