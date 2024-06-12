#ifndef SERIAL_H
#define SERIAL_H

#include "udong.h"

class SerialHandler {
  String serial_buffer;

  void HandleCmd(Udong& context, const String& cmd, const JsonDocument& arg);

 public:
  void HandleSerial(Udong& context);
};

#endif
