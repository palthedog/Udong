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

// A singleton Descriptor for RapidTriggerState
decaproto::Descriptor* kRapidTriggerState__Descriptor = nullptr;

const decaproto::Descriptor* RapidTriggerState::GetDescriptor() const {
    if (kRapidTriggerState__Descriptor != nullptr) {
        return kRapidTriggerState__Descriptor;
    }
    kRapidTriggerState__Descriptor = new decaproto::Descriptor();
    kRapidTriggerState__Descriptor->RegisterField(decaproto::FieldDescriptor(1, decaproto::FieldType::kDouble));
    kRapidTriggerState__Descriptor->RegisterField(decaproto::FieldDescriptor(2, decaproto::FieldType::kDouble));
    return kRapidTriggerState__Descriptor;
}

// A singleton Reflection object for RapidTriggerState
decaproto::Reflection* kRapidTriggerState__Reflection = nullptr;

const decaproto::Reflection* RapidTriggerState::GetReflection() const {
    if (kRapidTriggerState__Reflection != nullptr) {
        return kRapidTriggerState__Reflection;
    }
    kRapidTriggerState__Reflection = new decaproto::Reflection();
    
    // Setter
    kRapidTriggerState__Reflection->RegisterSetDouble(
        1,
		decaproto::MsgCast(&RapidTriggerState::set_release_point_mm));
    // Getter
    kRapidTriggerState__Reflection->RegisterGetDouble(
        1,
		decaproto::MsgCast(&RapidTriggerState::release_point_mm));
        
    // Setter
    kRapidTriggerState__Reflection->RegisterSetDouble(
        2,
		decaproto::MsgCast(&RapidTriggerState::set_actuation_point_mm));
    // Getter
    kRapidTriggerState__Reflection->RegisterGetDouble(
        2,
		decaproto::MsgCast(&RapidTriggerState::actuation_point_mm));
        return kRapidTriggerState__Reflection;
}

// A singleton Descriptor for StaticTriggerState
decaproto::Descriptor* kStaticTriggerState__Descriptor = nullptr;

const decaproto::Descriptor* StaticTriggerState::GetDescriptor() const {
    if (kStaticTriggerState__Descriptor != nullptr) {
        return kStaticTriggerState__Descriptor;
    }
    kStaticTriggerState__Descriptor = new decaproto::Descriptor();
    kStaticTriggerState__Descriptor->RegisterField(decaproto::FieldDescriptor(1, decaproto::FieldType::kDouble));
    kStaticTriggerState__Descriptor->RegisterField(decaproto::FieldDescriptor(2, decaproto::FieldType::kDouble));
    return kStaticTriggerState__Descriptor;
}

// A singleton Reflection object for StaticTriggerState
decaproto::Reflection* kStaticTriggerState__Reflection = nullptr;

const decaproto::Reflection* StaticTriggerState::GetReflection() const {
    if (kStaticTriggerState__Reflection != nullptr) {
        return kStaticTriggerState__Reflection;
    }
    kStaticTriggerState__Reflection = new decaproto::Reflection();
    
    // Setter
    kStaticTriggerState__Reflection->RegisterSetDouble(
        1,
		decaproto::MsgCast(&StaticTriggerState::set_release_point_mm));
    // Getter
    kStaticTriggerState__Reflection->RegisterGetDouble(
        1,
		decaproto::MsgCast(&StaticTriggerState::release_point_mm));
        
    // Setter
    kStaticTriggerState__Reflection->RegisterSetDouble(
        2,
		decaproto::MsgCast(&StaticTriggerState::set_actuation_point_mm));
    // Getter
    kStaticTriggerState__Reflection->RegisterGetDouble(
        2,
		decaproto::MsgCast(&StaticTriggerState::actuation_point_mm));
        return kStaticTriggerState__Reflection;
}

// A singleton Descriptor for AnalogSwitchState
decaproto::Descriptor* kAnalogSwitchState__Descriptor = nullptr;

const decaproto::Descriptor* AnalogSwitchState::GetDescriptor() const {
    if (kAnalogSwitchState__Descriptor != nullptr) {
        return kAnalogSwitchState__Descriptor;
    }
    kAnalogSwitchState__Descriptor = new decaproto::Descriptor();
    kAnalogSwitchState__Descriptor->RegisterField(decaproto::FieldDescriptor(1, decaproto::FieldType::kUint32));
    kAnalogSwitchState__Descriptor->RegisterField(decaproto::FieldDescriptor(3, decaproto::FieldType::kUint32));
    kAnalogSwitchState__Descriptor->RegisterField(decaproto::FieldDescriptor(2, decaproto::FieldType::kDouble));
    kAnalogSwitchState__Descriptor->RegisterField(decaproto::FieldDescriptor(4, decaproto::FieldType::kMessage));
    kAnalogSwitchState__Descriptor->RegisterField(decaproto::FieldDescriptor(5, decaproto::FieldType::kMessage));
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
    kAnalogSwitchState__Reflection->RegisterSetUint32(
        3,
		decaproto::MsgCast(&AnalogSwitchState::set_analog_switch_id));
    // Getter
    kAnalogSwitchState__Reflection->RegisterGetUint32(
        3,
		decaproto::MsgCast(&AnalogSwitchState::analog_switch_id));
        
    // Setter
    kAnalogSwitchState__Reflection->RegisterSetDouble(
        2,
		decaproto::MsgCast(&AnalogSwitchState::set_pressed_mm));
    // Getter
    kAnalogSwitchState__Reflection->RegisterGetDouble(
        2,
		decaproto::MsgCast(&AnalogSwitchState::pressed_mm));
        
    // Mutable getter for rapid_trigger
    kAnalogSwitchState__Reflection->RegisterMutableMessage(
        4,
		decaproto::MsgCast(&AnalogSwitchState::mutable_rapid_trigger));
    // Getter for rapid_trigger
    kAnalogSwitchState__Reflection->RegisterGetMessage(
        4,
		decaproto::MsgCast(&AnalogSwitchState::rapid_trigger));
    // Hazzer for rapid_trigger
    kAnalogSwitchState__Reflection->RegisterHasField(
        4,
		decaproto::MsgCast(&AnalogSwitchState::has_rapid_trigger));
    
    // Mutable getter for static_trigger
    kAnalogSwitchState__Reflection->RegisterMutableMessage(
        5,
		decaproto::MsgCast(&AnalogSwitchState::mutable_static_trigger));
    // Getter for static_trigger
    kAnalogSwitchState__Reflection->RegisterGetMessage(
        5,
		decaproto::MsgCast(&AnalogSwitchState::static_trigger));
    // Hazzer for static_trigger
    kAnalogSwitchState__Reflection->RegisterHasField(
        5,
		decaproto::MsgCast(&AnalogSwitchState::has_static_trigger));
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
