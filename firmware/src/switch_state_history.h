#ifndef STATE_HISTORY_H
#define STATE_HISTORY_H

#include <cstdint>
#include <vector>

#include "buffer/ring_buffer.h"
#include "proto/rpc.pb.h"
#include "udong.h"

class SwitchStateHistory {
  uint32_t active_analog_switch_id_;
  RingBuffer<AnalogSwitchState> analog_switch_state_history_;

 public:
  SwitchStateHistory()
      : active_analog_switch_id_(0), analog_switch_state_history_((size_t)32) {
  }

  uint32_t GetActiveAnalogSwitchId() const {
    return active_analog_switch_id_;
  }

  void PushAnalogSwitchState(Udong& udong) {
    AnalogSwitchState state;
    state.set_timestamp_us(time_us_32());

    AnalogSwitch* analog_switch =
        udong.GetAnalogSwitches()[active_analog_switch_id_].get();
    analog_switch->FillAnalogSwitchState(state);

    analog_switch_state_history_.PushBack(state);
  }

  void GetAnalogSwitchStateHistory(
      const GetAnalogSwitchStateRequest& request,
      GetAnalogSwitchStateResponse& response) {
    if (this->active_analog_switch_id_ != request.analog_switch_id()) {
      this->active_analog_switch_id_ = request.analog_switch_id();
      analog_switch_state_history_.Clear();
    }

    while (analog_switch_state_history_.Size() > 0) {
      AnalogSwitchState* state = response.add_states();
      analog_switch_state_history_.PopFront(*state);
    }
  }
};

#endif
