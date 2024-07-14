#pragma once

#include "decaproto/descriptor.h"
#include "decaproto/reflection.h"
#include "decaproto/field.h"
#include <memory>
#include <stdint.h>
#include <string>
#include <vector>
#include "decaproto/message.h"

class GetAnalogSwitchStateRequest;
class AnalogSwitchState;
class GetAnalogSwitchStateResponse;


class GetAnalogSwitchStateRequest final : public decaproto::Message {
public:
    GetAnalogSwitchStateRequest()
        : analog_switch_id__(uint32_t()) {}

    ~GetAnalogSwitchStateRequest() {}


	inline uint32_t analog_switch_id() const {
	    return analog_switch_id__;
	}

	inline void set_analog_switch_id( uint32_t value) {
	    analog_switch_id__ = value;
	}

	inline void clear_analog_switch_id() {
	    analog_switch_id__ = uint32_t();
	}
    const decaproto::Descriptor* GetDescriptor() const override;
    const decaproto::Reflection* GetReflection() const override;

private:
    uint32_t analog_switch_id__;


};

class AnalogSwitchState final : public decaproto::Message {
public:
    AnalogSwitchState()
        : timestamp_us__(uint32_t())
        , pressed_mm__(double()) {}

    ~AnalogSwitchState() {}


	inline uint32_t timestamp_us() const {
	    return timestamp_us__;
	}

	inline void set_timestamp_us( uint32_t value) {
	    timestamp_us__ = value;
	}

	inline void clear_timestamp_us() {
	    timestamp_us__ = uint32_t();
	}

	inline double pressed_mm() const {
	    return pressed_mm__;
	}

	inline void set_pressed_mm( double value) {
	    pressed_mm__ = value;
	}

	inline void clear_pressed_mm() {
	    pressed_mm__ = double();
	}
    const decaproto::Descriptor* GetDescriptor() const override;
    const decaproto::Reflection* GetReflection() const override;

private:
    uint32_t timestamp_us__;
    double pressed_mm__;


};

class GetAnalogSwitchStateResponse final : public decaproto::Message {
public:
    GetAnalogSwitchStateResponse()
        : states__() {}

    ~GetAnalogSwitchStateResponse() {}


	inline const std::vector<AnalogSwitchState>& states() const {
	    return states__;
	}

	inline const AnalogSwitchState& get_states(size_t index) const {
	    return states__[index];
	}

	inline size_t states_size() const {
	    return states__.size();
	}

	inline void set_states(size_t index, const AnalogSwitchState& value) {
	    states__[index] = value;
	}

	inline std::vector<AnalogSwitchState>* mutable_states() {
		return &states__;
	}

	inline AnalogSwitchState* add_states() {
	    states__.push_back(AnalogSwitchState());
		return &states__.back();
	}

	inline void clear_states() {
	    states__.clear();
	}

    const decaproto::Descriptor* GetDescriptor() const override;
    const decaproto::Reflection* GetReflection() const override;

private:
    std::vector<AnalogSwitchState> states__;


};


/*
name: "rpc.proto"
message_type: {
    name: "GetAnalogSwitchStateRequest"
    field: {
        name: "analog_switch_id"
        number: 1
        label: LABEL_OPTIONAL
        type: TYPE_UINT32
        json_name: "analogSwitchId"
    }
}
message_type: {
    name: "AnalogSwitchState"
    field: {
        name: "timestamp_us"
        number: 1
        label: LABEL_OPTIONAL
        type: TYPE_UINT32
        json_name: "timestampUs"
    }
    field: {
        name: "pressed_mm"
        number: 2
        label: LABEL_OPTIONAL
        type: TYPE_DOUBLE
        json_name: "pressedMm"
    }
}
message_type: {
    name: "GetAnalogSwitchStateResponse"
    field: {
        name: "states"
        number: 1
        label: LABEL_REPEATED
        type: TYPE_MESSAGE
        type_name: ".AnalogSwitchState"
        json_name: "states"
    }
}
source_code_info: {
    location: {
        span: 0
        span: 0
        span: 16
        span: 1
    }
    location: {
        path: 12
        span: 0
        span: 0
        span: 18
    }
    location: {
        path: 4
        path: 0
        span: 2
        span: 0
        span: 4
        span: 1
    }
    location: {
        path: 4
        path: 0
        path: 1
        span: 2
        span: 8
        span: 35
    }
    location: {
        path: 4
        path: 0
        path: 2
        path: 0
        span: 3
        span: 2
        span: 30
    }
    location: {
        path: 4
        path: 0
        path: 2
        path: 0
        path: 5
        span: 3
        span: 2
        span: 8
    }
    location: {
        path: 4
        path: 0
        path: 2
        path: 0
        path: 1
        span: 3
        span: 9
        span: 25
    }
    location: {
        path: 4
        path: 0
        path: 2
        path: 0
        path: 3
        span: 3
        span: 28
        span: 29
    }
    location: {
        path: 4
        path: 1
        span: 6
        span: 0
        span: 12
        span: 1
    }
    location: {
        path: 4
        path: 1
        path: 1
        span: 6
        span: 8
        span: 25
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 0
        span: 9
        span: 2
        span: 26
        leading_comments: " The timestamp in micro seconds.\n Note that this value may wrap roughly every 1 hour 11 minutes.\n"
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 0
        path: 5
        span: 9
        span: 2
        span: 8
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 0
        path: 1
        span: 9
        span: 9
        span: 21
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 0
        path: 3
        span: 9
        span: 24
        span: 25
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 1
        span: 11
        span: 2
        span: 24
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 1
        path: 5
        span: 11
        span: 2
        span: 8
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 1
        path: 1
        span: 11
        span: 9
        span: 19
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 1
        path: 3
        span: 11
        span: 22
        span: 23
    }
    location: {
        path: 4
        path: 2
        span: 14
        span: 0
        span: 16
        span: 1
    }
    location: {
        path: 4
        path: 2
        path: 1
        span: 14
        span: 8
        span: 36
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 0
        span: 15
        span: 2
        span: 40
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 0
        path: 4
        span: 15
        span: 2
        span: 10
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 0
        path: 6
        span: 15
        span: 11
        span: 28
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 0
        path: 1
        span: 15
        span: 29
        span: 35
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 0
        path: 3
        span: 15
        span: 38
        span: 39
    }
}
syntax: "proto3"
*/
