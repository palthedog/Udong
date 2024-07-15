#ifndef RAPID_TRIGGER_H
#define RAPID_TRIGGER_H

#include <cmath>

#include "trigger.h"

class RapidTrigger : public Trigger {
  double actuation_distance_mm_;
  double release_distance_mm_;

  double force_activation_point_depth_mm_;
  double force_release_point_depth_mm_;

  double deep_point_mm_;
  double shallow_point_mm_;

  bool current_state_;

 public:
  RapidTrigger(
      double actuation_distance_mm,
      double release_distance_mm,
      double force_activation_point_depth_mm,
      double force_release_point_depth_mm)
      : actuation_distance_mm_(actuation_distance_mm),
        release_distance_mm_(release_distance_mm),
        force_activation_point_depth_mm_(force_activation_point_depth_mm),
        force_release_point_depth_mm_(force_release_point_depth_mm),
        deep_point_mm_(0.0),
        shallow_point_mm_(0.0),
        current_state_(false) {
  }

  virtual ~RapidTrigger() {
  }

  virtual bool Triggered(double pressed_distance_mm) override {
    if (current_state_) {
      // Is already triggered
      if (pressed_distance_mm > deep_point_mm_) {
        // The user pressed even more
        deep_point_mm_ = pressed_distance_mm;
      } else {
        double release_point_mm = deep_point_mm_ - release_distance_mm_;
        if (pressed_distance_mm < release_point_mm ||
            pressed_distance_mm < force_release_point_depth_mm_) {
          // The user released more than `release_distance_mm`
          current_state_ = false;
          shallow_point_mm_ = pressed_distance_mm;
        }
      }
    } else {
      // Is NOT triggered yet

      // When the key is fully released, shallow point would be 0.0 mm
      //  0.0mm
      //   |
      // Shallow point
      //   X >>>>>> O
      //      Actuation point
      //
      // On the other hand, shallow point moves freely.
      // Bellow one illustrate a situation when the user is releasing the key
      //  0.0mm                  Released      activated
      //   |    Shallow point       X <<<<<<<<<<<< X
      //   |           X <<<<<<<<<< +
      //   |           |
      //   |           X >>>>>> O
      //   |              Actuation point = shallow + actuation_distance
      if (pressed_distance_mm < shallow_point_mm_) {
        // The user is releasing the key higher
        shallow_point_mm_ = pressed_distance_mm;
      } else {
        double actuation_point_mm = shallow_point_mm_ + actuation_distance_mm_;
        if (pressed_distance_mm > actuation_point_mm ||
            pressed_distance_mm > force_activation_point_depth_mm_) {
          current_state_ = true;
          deep_point_mm_ = pressed_distance_mm;
        }
      }
    }
    return current_state_;
  }

  virtual void FillTriggerState(AnalogSwitchState& state) const {
    RapidTriggerState& trigger_state = *state.mutable_rapid_trigger();
    trigger_state.set_actuation_point_mm(
        shallow_point_mm_ + actuation_distance_mm_);
    trigger_state.set_release_point_mm(deep_point_mm_ - release_distance_mm_);
  }

  virtual void TelePrint(int switch_id) const override {
#if TELEPLOT
    Serial.printf(
        ">act%02d:%lf\n",
        switch_id,
        shallow_point_mm_ + actuation_distance_mm_);
    Serial.printf(
        ">rel%02d:%lf\n", switch_id, deep_point_mm_ - release_distance_mm_);

    Serial.printf(">shallow%02d:%lf\n", switch_id, shallow_point_mm_);
    Serial.printf(">deep%02d:%lf\n", switch_id, deep_point_mm_);

    Serial.printf(
        ">fact%02d:%lf\n", switch_id, force_activation_point_depth_mm_);
    Serial.printf(">dead%02d:%lf\n", switch_id, force_release_point_depth_mm_);
    Serial.printf(">state%02d:%d\n", switch_id, current_state_ ? 4 : 0);
#endif
  }
};

#endif
