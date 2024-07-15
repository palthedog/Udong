#ifndef STATIC_TRIGGER_H
#define STATIC_TRIGGER_H

#include "trigger.h"

class StaticTrigger : public Trigger {
  double actuation_point_mm_;
  double release_point_mm_;

  bool current_state_;

 public:
  StaticTrigger(double actuation_point_mm, double release_point_mm)
      : actuation_point_mm_(actuation_point_mm),
        release_point_mm_(release_point_mm),
        current_state_(false) {
  }

  virtual ~StaticTrigger() {
  }

  virtual bool Triggered(double pressed_distance_mm) override {
    if (pressed_distance_mm >= actuation_point_mm_) {
      current_state_ = true;
    } else if (pressed_distance_mm <= release_point_mm_) {
      current_state_ = false;
    }
    return current_state_;
  }

  virtual void TelePrint(int switch_id) override {
#if TELEPLOT
    Serial.printf(">act%02d:%lf\n", switch_id, actuation_point_mm_);
    Serial.printf(">rel%02d:%lf\n", switch_id, release_point_mm_);
    Serial.printf(">state%02d:%d\n", switch_id, current_state_ ? 4 : 0);
#endif
  }
};

#endif
