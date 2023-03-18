#pragma once

constexpr int send_buffer_size = 1024*2;
constexpr int batch_size = 16;

enum class MessageHeaderType : char {
  send_filename ='n',
  data_buffer = 'd',
  data_batch = 'b',
  batch_accepted = 'a',
  send_folder = 'l',
  eot = 't',
  eof = 'f'
};

struct MessageHeader {
  MessageHeaderType type;
  int size;
};
