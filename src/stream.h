#ifndef STREAM_H
#define STREAM_H

#include "common.h"

namespace vm {

class Stream {
 public:
  class Client {
   public:
    virtual ~Client() { }
    virtual void NO_RETURN handleError() = 0;
  };

  Stream(Client* client, const uint8_t* data, unsigned size):
    client(client), data(data), size(size), position_(0)
  { }

  unsigned position() {
    return position_;
  }

  void setPosition(unsigned p) {
    position_ = p;
  }

  void skip(unsigned size) {
    if (size > this->size - position_) {
      client->handleError();
    } else {
      position_ += size;
    }
  }

  void read(uint8_t* data, unsigned size) {
    if (size > this->size - position_) {
      client->handleError();
    } else {
      memcpy(data, this->data + position_, size);
      position_ += size;
    }
  }

  uint8_t read1() {
    uint8_t v;
    read(&v, 1);
    return v;
  }

  uint16_t read2() {
    uint16_t a = read1();
    uint16_t b = read1();
    return (a << 8) | b;
  }

  uint32_t read4() {
    uint32_t a = read2();
    uint32_t b = read2();
    return (a << 16) | b;
  }

  uint64_t read8() {
    uint64_t a = read4();
    uint64_t b = read4();
    return (a << 32) | b;
  }

  uint32_t readFloat() {
    return read4();
  }

  uint64_t readDouble() {
    return read8();
  }

 private:
  Client* client;
  const uint8_t* data;
  unsigned size;
  unsigned position_;
};

} // namespace vm

#endif//STREAM_H
