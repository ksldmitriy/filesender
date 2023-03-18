#include "file_receiver.hpp"

FileReceiver::FileReceiver(ConnectionProrived *connection_provider)
  : IFileTransfer(connection_provider) {}

void FileReceiver::Run() {
  ReceiveLoop();
}

void FileReceiver::ReceiveLoop() {
  MessageHeader header;
  do {
    header = ReceiveMessage();
  } while (ProcessHeader(header));
}

bool FileReceiver::ProcessHeader(MessageHeader header) {
  if (state == State::wait_for_name) {
    return ProccessFilenameReceive(header);
  } else {
    ProccessDataReceive(header);
  }

  return true;
}

bool FileReceiver::ProccessFilenameReceive(MessageHeader header) {
  switch (header.type) {
  case MessageHeaderType::send_filename: {
    current_filename = ReceiveString() + "_rc";
    current_file.open(current_filename, ios::trunc | ios::binary);

    cout << "recv file: " << current_filename << endl;
    SendBatchAcception();

    state = State::wait_for_data;
    return true;
  } break;
  case MessageHeaderType::eot: {
    return false;
  } break;
  default: {
    cout << "invalid message name " << (char)header.type << endl;
  }
  }

  return true;
}

void FileReceiver::ProccessDataReceive(MessageHeader header) {
  switch (header.type) {
  case MessageHeaderType::data_buffer: {
    ReceiveBuffer(buffer, header.size);
    //    SendBatchAcception();
    current_file.write(buffer, header.size);
  } break;
  case MessageHeaderType::eof: {
    current_file.close();
    SendBatchAcception();
    state = State::wait_for_name;
  } break;
  default: {
    cout << "invalid message data " << (char)header.type << endl;
    // error
  }
  }
};

void FileReceiver::SendBatchAcception() {
  MessageHeader header;
  header.type = MessageHeaderType::batch_accepted;

  SendMessage(header);
}
