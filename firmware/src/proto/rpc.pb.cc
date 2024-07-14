#include "rpc.pb.h"

#include <cassert>
#include "decaproto/reflection_util.h"


// A singleton Descriptor for GetAnalogSwitchStateRequest
decaproto::Descriptor* kGetAnalogSwitchStateRequest__Descriptor = nullptr;

const decaproto::Descriptor* GetAnalogSwitchStateRequest::GetDescriptor() const {
    if (kGetAnalogSwitchStateRequest__Descriptor != nullptr) {
        return kGetAnalogSwitchStateRequest__Descriptor;
    }
    kGetAnalogSwitchStateRequest__Descriptor = new decaproto::Descriptor();
    kGetAnalogSwitchStateRequest__Descriptor->RegisterField(decaproto::FieldDescriptor(1, decaproto::FieldType::kUint32));
    return kGetAnalogSwitchStateRequest__Descriptor;
}

// A singleton Reflection object for GetAnalogSwitchStateRequest
decaproto::Reflection* kGetAnalogSwitchStateRequest__Reflection = nullptr;

const decaproto::Reflection* GetAnalogSwitchStateRequest::GetReflection() const {
    if (kGetAnalogSwitchStateRequest__Reflection != nullptr) {
        return kGetAnalogSwitchStateRequest__Reflection;
    }
    kGetAnalogSwitchStateRequest__Reflection = new decaproto::Reflection();
    
    // Setter
    kGetAnalogSwitchStateRequest__Reflection->RegisterSetUint32(
        1,
		decaproto::MsgCast(&GetAnalogSwitchStateRequest::set_analog_switch_id));
    // Getter
    kGetAnalogSwitchStateRequest__Reflection->RegisterGetUint32(
        1,
		decaproto::MsgCast(&GetAnalogSwitchStateRequest::analog_switch_id));
        return kGetAnalogSwitchStateRequest__Reflection;
}

// A singleton Descriptor for AnalogSwitchState
decaproto::Descriptor* kAnalogSwitchState__Descriptor = nullptr;

const decaproto::Descriptor* AnalogSwitchState::GetDescriptor() const {
    if (kAnalogSwitchState__Descriptor != nullptr) {
        return kAnalogSwitchState__Descriptor;
    }
    kAnalogSwitchState__Descriptor = new decaproto::Descriptor();
    kAnalogSwitchState__Descriptor->RegisterField(decaproto::FieldDescriptor(1, decaproto::FieldType::kUint32));
    kAnalogSwitchState__Descriptor->RegisterField(decaproto::FieldDescriptor(2, decaproto::FieldType::kDouble));
    return kAnalogSwitchState__Descriptor;
}

// A singleton Reflection object for AnalogSwitchState
decaproto::Reflection* kAnalogSwitchState__Reflection = nullptr;

const decaproto::Reflection* AnalogSwitchState::GetReflection() const {
    if (kAnalogSwitchState__Reflection != nullptr) {
        return kAnalogSwitchState__Reflection;
    }
    kAnalogSwitchState__Reflection = new decaproto::Reflection();
    
    // Setter
    kAnalogSwitchState__Reflection->RegisterSetUint32(
        1,
		decaproto::MsgCast(&AnalogSwitchState::set_timestamp_us));
    // Getter
    kAnalogSwitchState__Reflection->RegisterGetUint32(
        1,
		decaproto::MsgCast(&AnalogSwitchState::timestamp_us));
        
    // Setter
    kAnalogSwitchState__Reflection->RegisterSetDouble(
        2,
		decaproto::MsgCast(&AnalogSwitchState::set_pressed_mm));
    // Getter
    kAnalogSwitchState__Reflection->RegisterGetDouble(
        2,
		decaproto::MsgCast(&AnalogSwitchState::pressed_mm));
        return kAnalogSwitchState__Reflection;
}

// A singleton Descriptor for GetAnalogSwitchStateResponse
decaproto::Descriptor* kGetAnalogSwitchStateResponse__Descriptor = nullptr;

const decaproto::Descriptor* GetAnalogSwitchStateResponse::GetDescriptor() const {
    if (kGetAnalogSwitchStateResponse__Descriptor != nullptr) {
        return kGetAnalogSwitchStateResponse__Descriptor;
    }
    kGetAnalogSwitchStateResponse__Descriptor = new decaproto::Descriptor();
    kGetAnalogSwitchStateResponse__Descriptor->RegisterField(decaproto::FieldDescriptor(1, decaproto::FieldType::kMessage, true));
    return kGetAnalogSwitchStateResponse__Descriptor;
}

// A singleton Reflection object for GetAnalogSwitchStateResponse
decaproto::Reflection* kGetAnalogSwitchStateResponse__Reflection = nullptr;

const decaproto::Reflection* GetAnalogSwitchStateResponse::GetReflection() const {
    if (kGetAnalogSwitchStateResponse__Reflection != nullptr) {
        return kGetAnalogSwitchStateResponse__Reflection;
    }
    kGetAnalogSwitchStateResponse__Reflection = new decaproto::Reflection();
    
			// Mutable getter for states
			kGetAnalogSwitchStateResponse__Reflection->RegisterGetRepeatedMessage(
				1,
				decaproto::MsgCast(&GetAnalogSwitchStateResponse::get_states));
			kGetAnalogSwitchStateResponse__Reflection->RegisterAddRepeatedMessage(
				1,
				decaproto::MsgCast(&GetAnalogSwitchStateResponse::add_states));
			kGetAnalogSwitchStateResponse__Reflection->RegisterFieldSize(
				1,
				decaproto::MsgCast(&GetAnalogSwitchStateResponse::states_size));
		    return kGetAnalogSwitchStateResponse__Reflection;
}
