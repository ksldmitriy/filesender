#include "file_sender.hpp"

FileSender::FileSender(vector<fs::path> files, ConnectionProrived *connection_provider)
    : IFileTransfer(connection_provider) {
  this->files = files;
}

void FileSender::Run() {
  SendFiles();

  SendEOT();
}

void FileSender::SendFiles() {
  for (fs::path file : files) {
    SendFile(file);
  }
}

void FileSender::SendFile(fs::path filename) {
  cout << "sending file: " << filename << endl;

  MessageHeader header;
  header.type = MessageHeaderType::send_filename;
  SendMessage(header);
  SendString(filename.string());

  char buffer[send_buffer_size];
  ifstream file(filename, ios::binary);

  file.seekg(0, ios::end);
  int filesize = file.tellg();
  file.seekg(0, ios::beg);

  cout << "filesize = " << filesize << endl;

  time_point t1, t2;
  duration total_read_time_d = duration(0), total_send_time_d = duration(0);

  WaitBatchAcception();

  int total_read = 0;
  int read;
  do {
    t1 = chrono::high_resolution_clock::now();
    file.read(buffer, send_buffer_size);
    t2 = chrono::high_resolution_clock::now();
    total_read_time_d += t2 - t1;

    read = file.gcount();
    total_read += read;

    cout << total_read / (float)filesize << endl;

    // WaitBatchAcception();
    t1 = chrono::high_resolution_clock::now();
    SendBuffer(buffer, read);
    t2 = chrono::high_resolution_clock::now();
    total_send_time_d += t2 - t1;
  } while (read);

  double total_read_time =
      chrono::duration_cast<chrono::microseconds>(total_read_time_d).count() /
      1000.0;
  double total_send_time =
      chrono::duration_cast<chrono::microseconds>(total_send_time_d).count() /
      1000.0;

  cout << "total read time " << total_read_time << endl;
  cout << "total send time " << total_send_time << endl;

  SendEOF();
  WaitBatchAcception();
}

void FileSender::WaitBatchAcception() {
  MessageHeader header = ReceiveMessage();
  if (header.type != MessageHeaderType::batch_accepted) {
    // error
  }
}

void FileSender::SendEOT() {
  MessageHeader header;
  header.type = MessageHeaderType::eot;
  SendMessage(header);
}

void FileSender::SendEOF() {
  MessageHeader header;
  header.type = MessageHeaderType::eof;
  SendMessage(header);
}
