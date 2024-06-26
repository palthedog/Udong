#ifndef SERIAL_H
#define SERIAL_H

#include "udong.h"

// A class handling serial communication with Web-Configurator.
// Wire format is one of the following:
// - NO payload
//   command-name\n
//
//   A command name is followed by a newline (\n).
//
// - JSON
//   command-name:{ arg0: value0, arg1: value1, ... }\n
//
//   A commannd name and payload is delimited by a colon. The payload finishes
//   with a newline (\n).
//
// - Base64 encoded binary
//   command-name@100/...100 characters (which encodes payload)...
//
//   A command name followed by an atmark (@) and the length of the payload.
//   The payload is encoded in base64 and delimited by a slash (/).
//   Note that the length is
//   equal to `len(base64_encode(binary))`
//   NOT `len(binary)`
//
// - Bianry (not implemented yet)
//   command-name@100#...100 bytes of binary...
//
//   A command name followed by an atmark (@) and the length of the payload.
//   The payload is binary and delimited by a hash (#).
//
// Note that command-name is a string that consists of alphabets, numbers, and
// hyphen. ([a-zA-Z0-9_-]+)
class SerialHandler {
  enum State {
    kReadingCommand,
    kReadingJsonPayload,
    kReadingPayloadSize,
    kReadingBase64Payload,
    kReadingBinaryPayload,
  };

  State state_ = kReadingCommand;

  // A string buffer currently being parsed.
  std::vector<uint8_t> payload_buffer_;

  String command_;
  size_t payload_size_;

  void Reset() {
    state_ = kReadingCommand;
    payload_buffer_.clear();
    command_ = "";
    payload_size_ = 0;
  }

  void HandleCmd(Udong& context, const String& cmd);
  void HandleJsonCmd(
      Udong& context, const String& cmd, const JsonDocument& arg);

  void HandleBinaryCommand(Udong& context, const uint8_t* binary);

  void ReadCommand(Udong& context);
  void ReadJsonPayload(Udong& context);
  void ReadPayloadSize(Udong& context);
  void ReadBinaryPayload(Udong& context);
  void ReadBase64Payload(Udong& context);

 public:
  void HandleSerial(Udong& context);
};

#endif
