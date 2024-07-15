#pragma once

#include <string>
#include <vector>
#include "decaproto/message.h"
#include "decaproto/descriptor.h"
#include "decaproto/reflection.h"
#include "decaproto/field.h"
#include <memory>
#include <stdint.h>

class GetAnalogSwitchStateRequest;
class RapidTriggerState;
class StaticTriggerState;
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

class RapidTriggerState final : public decaproto::Message {
public:
    RapidTriggerState()
        : release_point_mm__(double())
        , actuation_point_mm__(double()) {}

    ~RapidTriggerState() {}


	inline double release_point_mm() const {
	    return release_point_mm__;
	}

	inline void set_release_point_mm( double value) {
	    release_point_mm__ = value;
	}

	inline void clear_release_point_mm() {
	    release_point_mm__ = double();
	}

	inline double actuation_point_mm() const {
	    return actuation_point_mm__;
	}

	inline void set_actuation_point_mm( double value) {
	    actuation_point_mm__ = value;
	}

	inline void clear_actuation_point_mm() {
	    actuation_point_mm__ = double();
	}
    const decaproto::Descriptor* GetDescriptor() const override;
    const decaproto::Reflection* GetReflection() const override;

private:
    double release_point_mm__;
    double actuation_point_mm__;


};

class StaticTriggerState final : public decaproto::Message {
public:
    StaticTriggerState()
        : release_point_mm__(double())
        , actuation_point_mm__(double()) {}

    ~StaticTriggerState() {}


	inline double release_point_mm() const {
	    return release_point_mm__;
	}

	inline void set_release_point_mm( double value) {
	    release_point_mm__ = value;
	}

	inline void clear_release_point_mm() {
	    release_point_mm__ = double();
	}

	inline double actuation_point_mm() const {
	    return actuation_point_mm__;
	}

	inline void set_actuation_point_mm( double value) {
	    actuation_point_mm__ = value;
	}

	inline void clear_actuation_point_mm() {
	    actuation_point_mm__ = double();
	}
    const decaproto::Descriptor* GetDescriptor() const override;
    const decaproto::Reflection* GetReflection() const override;

private:
    double release_point_mm__;
    double actuation_point_mm__;


};

class AnalogSwitchState final : public decaproto::Message {
public:
    AnalogSwitchState()
        : timestamp_us__(uint32_t())
        , analog_switch_id__(uint32_t())
        , pressed_mm__(double())
        , is_triggered__(bool())
        , rapid_trigger__()
        , has_rapid_trigger__(false)
        , static_trigger__()
        , has_static_trigger__(false) {}

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

	inline uint32_t analog_switch_id() const {
	    return analog_switch_id__;
	}

	inline void set_analog_switch_id( uint32_t value) {
	    analog_switch_id__ = value;
	}

	inline void clear_analog_switch_id() {
	    analog_switch_id__ = uint32_t();
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

	inline bool is_triggered() const {
	    return is_triggered__;
	}

	inline void set_is_triggered( bool value) {
	    is_triggered__ = value;
	}

	inline void clear_is_triggered() {
	    is_triggered__ = bool();
	}

	// Getter for rapid_trigger
	const RapidTriggerState& rapid_trigger() const {
		if (!rapid_trigger__) {
			rapid_trigger__.resetDefault();
        }
	    return *rapid_trigger__;
	}

	// Mutable Getter for rapid_trigger
	RapidTriggerState* mutable_rapid_trigger() {
	    if (!rapid_trigger__) {
			rapid_trigger__.resetDefault();
        }
        has_rapid_trigger__ = true;
	    return rapid_trigger__.get();
	}

	// Hazzer for rapid_trigger
	bool has_rapid_trigger() const {
	    return has_rapid_trigger__;
	}

	// Clearer for rapid_trigger
	void clear_rapid_trigger() {
	    rapid_trigger__.reset();
		has_rapid_trigger__ = false;
	}


	// Getter for static_trigger
	const StaticTriggerState& static_trigger() const {
		if (!static_trigger__) {
			static_trigger__.resetDefault();
        }
	    return *static_trigger__;
	}

	// Mutable Getter for static_trigger
	StaticTriggerState* mutable_static_trigger() {
	    if (!static_trigger__) {
			static_trigger__.resetDefault();
        }
        has_static_trigger__ = true;
	    return static_trigger__.get();
	}

	// Hazzer for static_trigger
	bool has_static_trigger() const {
	    return has_static_trigger__;
	}

	// Clearer for static_trigger
	void clear_static_trigger() {
	    static_trigger__.reset();
		has_static_trigger__ = false;
	}

    const decaproto::Descriptor* GetDescriptor() const override;
    const decaproto::Reflection* GetReflection() const override;

private:
    uint32_t timestamp_us__;
    uint32_t analog_switch_id__;
    double pressed_mm__;
    bool is_triggered__;
    mutable decaproto::SubMessagePtr<RapidTriggerState> rapid_trigger__;
    bool has_rapid_trigger__;
    mutable decaproto::SubMessagePtr<StaticTriggerState> static_trigger__;
    bool has_static_trigger__;


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
    name: "RapidTriggerState"
    field: {
        name: "release_point_mm"
        number: 1
        label: LABEL_OPTIONAL
        type: TYPE_DOUBLE
        json_name: "releasePointMm"
    }
    field: {
        name: "actuation_point_mm"
        number: 2
        label: LABEL_OPTIONAL
        type: TYPE_DOUBLE
        json_name: "actuationPointMm"
    }
}
message_type: {
    name: "StaticTriggerState"
    field: {
        name: "release_point_mm"
        number: 1
        label: LABEL_OPTIONAL
        type: TYPE_DOUBLE
        json_name: "releasePointMm"
    }
    field: {
        name: "actuation_point_mm"
        number: 2
        label: LABEL_OPTIONAL
        type: TYPE_DOUBLE
        json_name: "actuationPointMm"
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
        name: "analog_switch_id"
        number: 3
        label: LABEL_OPTIONAL
        type: TYPE_UINT32
        json_name: "analogSwitchId"
    }
    field: {
        name: "pressed_mm"
        number: 2
        label: LABEL_OPTIONAL
        type: TYPE_DOUBLE
        json_name: "pressedMm"
    }
    field: {
        name: "is_triggered"
        number: 6
        label: LABEL_OPTIONAL
        type: TYPE_BOOL
        json_name: "isTriggered"
    }
    field: {
        name: "rapid_trigger"
        number: 4
        label: LABEL_OPTIONAL
        type: TYPE_MESSAGE
        type_name: ".RapidTriggerState"
        oneof_index: 0
        json_name: "rapidTrigger"
    }
    field: {
        name: "static_trigger"
        number: 5
        label: LABEL_OPTIONAL
        type: TYPE_MESSAGE
        type_name: ".StaticTriggerState"
        oneof_index: 0
        json_name: "staticTrigger"
    }
    oneof_decl: {
        name: "trigger"
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
        span: 35
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
        span: 9
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
        span: 7
        span: 2
        span: 30
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 0
        path: 5
        span: 7
        span: 2
        span: 8
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 0
        path: 1
        span: 7
        span: 9
        span: 25
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 0
        path: 3
        span: 7
        span: 28
        span: 29
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 1
        span: 8
        span: 2
        span: 32
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 1
        path: 5
        span: 8
        span: 2
        span: 8
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 1
        path: 1
        span: 8
        span: 9
        span: 27
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 1
        path: 3
        span: 8
        span: 30
        span: 31
    }
    location: {
        path: 4
        path: 2
        span: 11
        span: 0
        span: 14
        span: 1
    }
    location: {
        path: 4
        path: 2
        path: 1
        span: 11
        span: 8
        span: 26
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 0
        span: 12
        span: 2
        span: 30
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 0
        path: 5
        span: 12
        span: 2
        span: 8
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 0
        path: 1
        span: 12
        span: 9
        span: 25
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 0
        path: 3
        span: 12
        span: 28
        span: 29
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 1
        span: 13
        span: 2
        span: 32
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 1
        path: 5
        span: 13
        span: 2
        span: 8
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 1
        path: 1
        span: 13
        span: 9
        span: 27
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 1
        path: 3
        span: 13
        span: 30
        span: 31
    }
    location: {
        path: 4
        path: 3
        span: 16
        span: 0
        span: 31
        span: 1
    }
    location: {
        path: 4
        path: 3
        path: 1
        span: 16
        span: 8
        span: 25
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 0
        span: 19
        span: 2
        span: 26
        leading_comments: " The timestamp in micro seconds.\n Note that this value may wrap roughly every 1 hour 11 minutes.\n"
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 0
        path: 5
        span: 19
        span: 2
        span: 8
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 0
        path: 1
        span: 19
        span: 9
        span: 21
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 0
        path: 3
        span: 19
        span: 24
        span: 25
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 1
        span: 22
        span: 2
        span: 30
        leading_comments: " The analog switch_id associated with this state\n"
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 1
        path: 5
        span: 22
        span: 2
        span: 8
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 1
        path: 1
        span: 22
        span: 9
        span: 25
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 1
        path: 3
        span: 22
        span: 28
        span: 29
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 2
        span: 24
        span: 2
        span: 24
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 2
        path: 5
        span: 24
        span: 2
        span: 8
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 2
        path: 1
        span: 24
        span: 9
        span: 19
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 2
        path: 3
        span: 24
        span: 22
        span: 23
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 3
        span: 25
        span: 2
        span: 24
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 3
        path: 5
        span: 25
        span: 2
        span: 6
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 3
        path: 1
        span: 25
        span: 7
        span: 19
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 3
        path: 3
        span: 25
        span: 22
        span: 23
    }
    location: {
        path: 4
        path: 3
        path: 8
        path: 0
        span: 27
        span: 2
        span: 30
        span: 3
    }
    location: {
        path: 4
        path: 3
        path: 8
        path: 0
        path: 1
        span: 27
        span: 8
        span: 15
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 4
        span: 28
        span: 4
        span: 40
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 4
        path: 6
        span: 28
        span: 4
        span: 21
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 4
        path: 1
        span: 28
        span: 22
        span: 35
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 4
        path: 3
        span: 28
        span: 38
        span: 39
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 5
        span: 29
        span: 4
        span: 42
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 5
        path: 6
        span: 29
        span: 4
        span: 22
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 5
        path: 1
        span: 29
        span: 23
        span: 37
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 5
        path: 3
        span: 29
        span: 40
        span: 41
    }
    location: {
        path: 4
        path: 4
        span: 33
        span: 0
        span: 35
        span: 1
    }
    location: {
        path: 4
        path: 4
        path: 1
        span: 33
        span: 8
        span: 36
    }
    location: {
        path: 4
        path: 4
        path: 2
        path: 0
        span: 34
        span: 2
        span: 40
    }
    location: {
        path: 4
        path: 4
        path: 2
        path: 0
        path: 4
        span: 34
        span: 2
        span: 10
    }
    location: {
        path: 4
        path: 4
        path: 2
        path: 0
        path: 6
        span: 34
        span: 11
        span: 28
    }
    location: {
        path: 4
        path: 4
        path: 2
        path: 0
        path: 1
        span: 34
        span: 29
        span: 35
    }
    location: {
        path: 4
        path: 4
        path: 2
        path: 0
        path: 3
        span: 34
        span: 38
        span: 39
    }
}
syntax: "proto3"
*/
