#ifndef TRIGGER_H
#define TRIGGER_H

#include "proto/rpc.pb.h"

class Trigger {
 public:
  virtual ~Trigger() {
  }

  virtual bool Triggered(double travel_distance_mm) = 0;
  virtual void TelePrint(int switch_id) const = 0;
  virtual void FillTriggerState(AnalogSwitchState& state) const = 0;
};

#endif
