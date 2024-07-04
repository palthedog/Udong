#include "config.pb.h"

#include <cassert>
#include "decaproto/reflection_util.h"


// A singleton Descriptor for AnalogSwitchAssignment
decaproto::Descriptor* kAnalogSwitchAssignment__Descriptor = nullptr;

const decaproto::Descriptor* AnalogSwitchAssignment::GetDescriptor() const {
    if (kAnalogSwitchAssignment__Descriptor != nullptr) {
        return kAnalogSwitchAssignment__Descriptor;
    }
    kAnalogSwitchAssignment__Descriptor = new decaproto::Descriptor();
    kAnalogSwitchAssignment__Descriptor->RegisterField(decaproto::FieldDescriptor(1, decaproto::FieldType::kUint32));
    kAnalogSwitchAssignment__Descriptor->RegisterField(decaproto::FieldDescriptor(2, decaproto::FieldType::kUint32));
    return kAnalogSwitchAssignment__Descriptor;
}

// A singleton Reflection object for AnalogSwitchAssignment
decaproto::Reflection* kAnalogSwitchAssignment__Reflection = nullptr;

const decaproto::Reflection* AnalogSwitchAssignment::GetReflection() const {
    if (kAnalogSwitchAssignment__Reflection != nullptr) {
        return kAnalogSwitchAssignment__Reflection;
    }
    kAnalogSwitchAssignment__Reflection = new decaproto::Reflection();
    
    // Setter
    kAnalogSwitchAssignment__Reflection->RegisterSetUint32(
        1,
		decaproto::MsgCast(&AnalogSwitchAssignment::set_analog_switch_id));
    // Getter
    kAnalogSwitchAssignment__Reflection->RegisterGetUint32(
        1,
		decaproto::MsgCast(&AnalogSwitchAssignment::analog_switch_id));
        
    // Setter
    kAnalogSwitchAssignment__Reflection->RegisterSetUint32(
        2,
		decaproto::MsgCast(&AnalogSwitchAssignment::set_analog_switch_group_id));
    // Getter
    kAnalogSwitchAssignment__Reflection->RegisterGetUint32(
        2,
		decaproto::MsgCast(&AnalogSwitchAssignment::analog_switch_group_id));
        return kAnalogSwitchAssignment__Reflection;
}

// A singleton Descriptor for AnalogSwitchGroup
decaproto::Descriptor* kAnalogSwitchGroup__Descriptor = nullptr;

const decaproto::Descriptor* AnalogSwitchGroup::GetDescriptor() const {
    if (kAnalogSwitchGroup__Descriptor != nullptr) {
        return kAnalogSwitchGroup__Descriptor;
    }
    kAnalogSwitchGroup__Descriptor = new decaproto::Descriptor();
    kAnalogSwitchGroup__Descriptor->RegisterField(decaproto::FieldDescriptor(1, decaproto::FieldType::kUint32));
    kAnalogSwitchGroup__Descriptor->RegisterField(decaproto::FieldDescriptor(2, decaproto::FieldType::kEnum));
    kAnalogSwitchGroup__Descriptor->RegisterField(decaproto::FieldDescriptor(3, decaproto::FieldType::kMessage));
    kAnalogSwitchGroup__Descriptor->RegisterField(decaproto::FieldDescriptor(4, decaproto::FieldType::kMessage));
    return kAnalogSwitchGroup__Descriptor;
}

// A singleton Reflection object for AnalogSwitchGroup
decaproto::Reflection* kAnalogSwitchGroup__Reflection = nullptr;

const decaproto::Reflection* AnalogSwitchGroup::GetReflection() const {
    if (kAnalogSwitchGroup__Reflection != nullptr) {
        return kAnalogSwitchGroup__Reflection;
    }
    kAnalogSwitchGroup__Reflection = new decaproto::Reflection();
    
    // Setter
    kAnalogSwitchGroup__Reflection->RegisterSetUint32(
        1,
		decaproto::MsgCast(&AnalogSwitchGroup::set_analog_switch_group_id));
    // Getter
    kAnalogSwitchGroup__Reflection->RegisterGetUint32(
        1,
		decaproto::MsgCast(&AnalogSwitchGroup::analog_switch_group_id));
        
    // EnumValue setter for trigger_type
     kAnalogSwitchGroup__Reflection->RegisterSetEnumValue(
        2,
		decaproto::CastForSetEnumValue(&AnalogSwitchGroup::set_trigger_type));
     // EnumValue getter for trigger_type
     kAnalogSwitchGroup__Reflection->RegisterGetEnumValue(
        2,
		decaproto::CastForGetEnumValue(&AnalogSwitchGroup::trigger_type));
    
    // Mutable getter for rapid_trigger
    kAnalogSwitchGroup__Reflection->RegisterMutableMessage(
        3,
		decaproto::MsgCast(&AnalogSwitchGroup::mutable_rapid_trigger));
    // Getter for rapid_trigger
    kAnalogSwitchGroup__Reflection->RegisterGetMessage(
        3,
		decaproto::MsgCast(&AnalogSwitchGroup::rapid_trigger));
    // Hazzer for rapid_trigger
    kAnalogSwitchGroup__Reflection->RegisterHasField(
        3,
		decaproto::MsgCast(&AnalogSwitchGroup::has_rapid_trigger));
    
    // Mutable getter for static_trigger
    kAnalogSwitchGroup__Reflection->RegisterMutableMessage(
        4,
		decaproto::MsgCast(&AnalogSwitchGroup::mutable_static_trigger));
    // Getter for static_trigger
    kAnalogSwitchGroup__Reflection->RegisterGetMessage(
        4,
		decaproto::MsgCast(&AnalogSwitchGroup::static_trigger));
    // Hazzer for static_trigger
    kAnalogSwitchGroup__Reflection->RegisterHasField(
        4,
		decaproto::MsgCast(&AnalogSwitchGroup::has_static_trigger));
    return kAnalogSwitchGroup__Reflection;
}

// A singleton Descriptor for RapidTriggerConfig
decaproto::Descriptor* kRapidTriggerConfig__Descriptor = nullptr;

const decaproto::Descriptor* RapidTriggerConfig::GetDescriptor() const {
    if (kRapidTriggerConfig__Descriptor != nullptr) {
        return kRapidTriggerConfig__Descriptor;
    }
    kRapidTriggerConfig__Descriptor = new decaproto::Descriptor();
    kRapidTriggerConfig__Descriptor->RegisterField(decaproto::FieldDescriptor(1, decaproto::FieldType::kDouble));
    kRapidTriggerConfig__Descriptor->RegisterField(decaproto::FieldDescriptor(2, decaproto::FieldType::kDouble));
    kRapidTriggerConfig__Descriptor->RegisterField(decaproto::FieldDescriptor(3, decaproto::FieldType::kDouble));
    kRapidTriggerConfig__Descriptor->RegisterField(decaproto::FieldDescriptor(4, decaproto::FieldType::kDouble));
    return kRapidTriggerConfig__Descriptor;
}

// A singleton Reflection object for RapidTriggerConfig
decaproto::Reflection* kRapidTriggerConfig__Reflection = nullptr;

const decaproto::Reflection* RapidTriggerConfig::GetReflection() const {
    if (kRapidTriggerConfig__Reflection != nullptr) {
        return kRapidTriggerConfig__Reflection;
    }
    kRapidTriggerConfig__Reflection = new decaproto::Reflection();
    
    // Setter
    kRapidTriggerConfig__Reflection->RegisterSetDouble(
        1,
		decaproto::MsgCast(&RapidTriggerConfig::set_act));
    // Getter
    kRapidTriggerConfig__Reflection->RegisterGetDouble(
        1,
		decaproto::MsgCast(&RapidTriggerConfig::act));
        
    // Setter
    kRapidTriggerConfig__Reflection->RegisterSetDouble(
        2,
		decaproto::MsgCast(&RapidTriggerConfig::set_rel));
    // Getter
    kRapidTriggerConfig__Reflection->RegisterGetDouble(
        2,
		decaproto::MsgCast(&RapidTriggerConfig::rel));
        
    // Setter
    kRapidTriggerConfig__Reflection->RegisterSetDouble(
        3,
		decaproto::MsgCast(&RapidTriggerConfig::set_f_act));
    // Getter
    kRapidTriggerConfig__Reflection->RegisterGetDouble(
        3,
		decaproto::MsgCast(&RapidTriggerConfig::f_act));
        
    // Setter
    kRapidTriggerConfig__Reflection->RegisterSetDouble(
        4,
		decaproto::MsgCast(&RapidTriggerConfig::set_f_rel));
    // Getter
    kRapidTriggerConfig__Reflection->RegisterGetDouble(
        4,
		decaproto::MsgCast(&RapidTriggerConfig::f_rel));
        return kRapidTriggerConfig__Reflection;
}

// A singleton Descriptor for StaticTriggerConfig
decaproto::Descriptor* kStaticTriggerConfig__Descriptor = nullptr;

const decaproto::Descriptor* StaticTriggerConfig::GetDescriptor() const {
    if (kStaticTriggerConfig__Descriptor != nullptr) {
        return kStaticTriggerConfig__Descriptor;
    }
    kStaticTriggerConfig__Descriptor = new decaproto::Descriptor();
    kStaticTriggerConfig__Descriptor->RegisterField(decaproto::FieldDescriptor(1, decaproto::FieldType::kDouble));
    kStaticTriggerConfig__Descriptor->RegisterField(decaproto::FieldDescriptor(2, decaproto::FieldType::kDouble));
    return kStaticTriggerConfig__Descriptor;
}

// A singleton Reflection object for StaticTriggerConfig
decaproto::Reflection* kStaticTriggerConfig__Reflection = nullptr;

const decaproto::Reflection* StaticTriggerConfig::GetReflection() const {
    if (kStaticTriggerConfig__Reflection != nullptr) {
        return kStaticTriggerConfig__Reflection;
    }
    kStaticTriggerConfig__Reflection = new decaproto::Reflection();
    
    // Setter
    kStaticTriggerConfig__Reflection->RegisterSetDouble(
        1,
		decaproto::MsgCast(&StaticTriggerConfig::set_act));
    // Getter
    kStaticTriggerConfig__Reflection->RegisterGetDouble(
        1,
		decaproto::MsgCast(&StaticTriggerConfig::act));
        
    // Setter
    kStaticTriggerConfig__Reflection->RegisterSetDouble(
        2,
		decaproto::MsgCast(&StaticTriggerConfig::set_rel));
    // Getter
    kStaticTriggerConfig__Reflection->RegisterGetDouble(
        2,
		decaproto::MsgCast(&StaticTriggerConfig::rel));
        return kStaticTriggerConfig__Reflection;
}

// A singleton Descriptor for ButtonId
decaproto::Descriptor* kButtonId__Descriptor = nullptr;

const decaproto::Descriptor* ButtonId::GetDescriptor() const {
    if (kButtonId__Descriptor != nullptr) {
        return kButtonId__Descriptor;
    }
    kButtonId__Descriptor = new decaproto::Descriptor();
    kButtonId__Descriptor->RegisterField(decaproto::FieldDescriptor(1, decaproto::FieldType::kEnum));
    kButtonId__Descriptor->RegisterField(decaproto::FieldDescriptor(2, decaproto::FieldType::kMessage));
    kButtonId__Descriptor->RegisterField(decaproto::FieldDescriptor(3, decaproto::FieldType::kMessage));
    return kButtonId__Descriptor;
}

// A singleton Reflection object for ButtonId
decaproto::Reflection* kButtonId__Reflection = nullptr;

const decaproto::Reflection* ButtonId::GetReflection() const {
    if (kButtonId__Reflection != nullptr) {
        return kButtonId__Reflection;
    }
    kButtonId__Reflection = new decaproto::Reflection();
    
    // EnumValue setter for type
     kButtonId__Reflection->RegisterSetEnumValue(
        1,
		decaproto::CastForSetEnumValue(&ButtonId::set_type));
     // EnumValue getter for type
     kButtonId__Reflection->RegisterGetEnumValue(
        1,
		decaproto::CastForGetEnumValue(&ButtonId::type));
    
    // Mutable getter for push_button
    kButtonId__Reflection->RegisterMutableMessage(
        2,
		decaproto::MsgCast(&ButtonId::mutable_push_button));
    // Getter for push_button
    kButtonId__Reflection->RegisterGetMessage(
        2,
		decaproto::MsgCast(&ButtonId::push_button));
    // Hazzer for push_button
    kButtonId__Reflection->RegisterHasField(
        2,
		decaproto::MsgCast(&ButtonId::has_push_button));
    
    // Mutable getter for d_pad
    kButtonId__Reflection->RegisterMutableMessage(
        3,
		decaproto::MsgCast(&ButtonId::mutable_d_pad));
    // Getter for d_pad
    kButtonId__Reflection->RegisterGetMessage(
        3,
		decaproto::MsgCast(&ButtonId::d_pad));
    // Hazzer for d_pad
    kButtonId__Reflection->RegisterHasField(
        3,
		decaproto::MsgCast(&ButtonId::has_d_pad));
    return kButtonId__Reflection;
}

// A singleton Descriptor for PushButtonSelector
decaproto::Descriptor* kPushButtonSelector__Descriptor = nullptr;

const decaproto::Descriptor* PushButtonSelector::GetDescriptor() const {
    if (kPushButtonSelector__Descriptor != nullptr) {
        return kPushButtonSelector__Descriptor;
    }
    kPushButtonSelector__Descriptor = new decaproto::Descriptor();
    kPushButtonSelector__Descriptor->RegisterField(decaproto::FieldDescriptor(1, decaproto::FieldType::kUint32));
    return kPushButtonSelector__Descriptor;
}

// A singleton Reflection object for PushButtonSelector
decaproto::Reflection* kPushButtonSelector__Reflection = nullptr;

const decaproto::Reflection* PushButtonSelector::GetReflection() const {
    if (kPushButtonSelector__Reflection != nullptr) {
        return kPushButtonSelector__Reflection;
    }
    kPushButtonSelector__Reflection = new decaproto::Reflection();
    
    // Setter
    kPushButtonSelector__Reflection->RegisterSetUint32(
        1,
		decaproto::MsgCast(&PushButtonSelector::set_push_button_id));
    // Getter
    kPushButtonSelector__Reflection->RegisterGetUint32(
        1,
		decaproto::MsgCast(&PushButtonSelector::push_button_id));
        return kPushButtonSelector__Reflection;
}

// A singleton Descriptor for DPadButtonSelector
decaproto::Descriptor* kDPadButtonSelector__Descriptor = nullptr;

const decaproto::Descriptor* DPadButtonSelector::GetDescriptor() const {
    if (kDPadButtonSelector__Descriptor != nullptr) {
        return kDPadButtonSelector__Descriptor;
    }
    kDPadButtonSelector__Descriptor = new decaproto::Descriptor();
    kDPadButtonSelector__Descriptor->RegisterField(decaproto::FieldDescriptor(1, decaproto::FieldType::kEnum));
    return kDPadButtonSelector__Descriptor;
}

// A singleton Reflection object for DPadButtonSelector
decaproto::Reflection* kDPadButtonSelector__Reflection = nullptr;

const decaproto::Reflection* DPadButtonSelector::GetReflection() const {
    if (kDPadButtonSelector__Reflection != nullptr) {
        return kDPadButtonSelector__Reflection;
    }
    kDPadButtonSelector__Reflection = new decaproto::Reflection();
    
    // EnumValue setter for direction
     kDPadButtonSelector__Reflection->RegisterSetEnumValue(
        1,
		decaproto::CastForSetEnumValue(&DPadButtonSelector::set_direction));
     // EnumValue getter for direction
     kDPadButtonSelector__Reflection->RegisterGetEnumValue(
        1,
		decaproto::CastForGetEnumValue(&DPadButtonSelector::direction));
    return kDPadButtonSelector__Reflection;
}

// A singleton Descriptor for ButtonAssignment
decaproto::Descriptor* kButtonAssignment__Descriptor = nullptr;

const decaproto::Descriptor* ButtonAssignment::GetDescriptor() const {
    if (kButtonAssignment__Descriptor != nullptr) {
        return kButtonAssignment__Descriptor;
    }
    kButtonAssignment__Descriptor = new decaproto::Descriptor();
    kButtonAssignment__Descriptor->RegisterField(decaproto::FieldDescriptor(1, decaproto::FieldType::kUint32));
    kButtonAssignment__Descriptor->RegisterField(decaproto::FieldDescriptor(2, decaproto::FieldType::kMessage));
    return kButtonAssignment__Descriptor;
}

// A singleton Reflection object for ButtonAssignment
decaproto::Reflection* kButtonAssignment__Reflection = nullptr;

const decaproto::Reflection* ButtonAssignment::GetReflection() const {
    if (kButtonAssignment__Reflection != nullptr) {
        return kButtonAssignment__Reflection;
    }
    kButtonAssignment__Reflection = new decaproto::Reflection();
    
    // Setter
    kButtonAssignment__Reflection->RegisterSetUint32(
        1,
		decaproto::MsgCast(&ButtonAssignment::set_switch_id));
    // Getter
    kButtonAssignment__Reflection->RegisterGetUint32(
        1,
		decaproto::MsgCast(&ButtonAssignment::switch_id));
        
    // Mutable getter for button_id
    kButtonAssignment__Reflection->RegisterMutableMessage(
        2,
		decaproto::MsgCast(&ButtonAssignment::mutable_button_id));
    // Getter for button_id
    kButtonAssignment__Reflection->RegisterGetMessage(
        2,
		decaproto::MsgCast(&ButtonAssignment::button_id));
    // Hazzer for button_id
    kButtonAssignment__Reflection->RegisterHasField(
        2,
		decaproto::MsgCast(&ButtonAssignment::has_button_id));
    return kButtonAssignment__Reflection;
}

// A singleton Descriptor for UdongConfig
decaproto::Descriptor* kUdongConfig__Descriptor = nullptr;

const decaproto::Descriptor* UdongConfig::GetDescriptor() const {
    if (kUdongConfig__Descriptor != nullptr) {
        return kUdongConfig__Descriptor;
    }
    kUdongConfig__Descriptor = new decaproto::Descriptor();
    kUdongConfig__Descriptor->RegisterField(decaproto::FieldDescriptor(1, decaproto::FieldType::kMessage, true));
    kUdongConfig__Descriptor->RegisterField(decaproto::FieldDescriptor(2, decaproto::FieldType::kMessage, true));
    kUdongConfig__Descriptor->RegisterField(decaproto::FieldDescriptor(3, decaproto::FieldType::kMessage, true));
    return kUdongConfig__Descriptor;
}

// A singleton Reflection object for UdongConfig
decaproto::Reflection* kUdongConfig__Reflection = nullptr;

const decaproto::Reflection* UdongConfig::GetReflection() const {
    if (kUdongConfig__Reflection != nullptr) {
        return kUdongConfig__Reflection;
    }
    kUdongConfig__Reflection = new decaproto::Reflection();
    
			// Mutable getter for analog_switch_assignments
			kUdongConfig__Reflection->RegisterGetRepeatedMessage(
				1,
				decaproto::MsgCast(&UdongConfig::get_analog_switch_assignments));
			kUdongConfig__Reflection->RegisterAddRepeatedMessage(
				1,
				decaproto::MsgCast(&UdongConfig::add_analog_switch_assignments));
			kUdongConfig__Reflection->RegisterFieldSize(
				1,
				decaproto::MsgCast(&UdongConfig::analog_switch_assignments_size));
		    
			// Mutable getter for analog_switch_groups
			kUdongConfig__Reflection->RegisterGetRepeatedMessage(
				2,
				decaproto::MsgCast(&UdongConfig::get_analog_switch_groups));
			kUdongConfig__Reflection->RegisterAddRepeatedMessage(
				2,
				decaproto::MsgCast(&UdongConfig::add_analog_switch_groups));
			kUdongConfig__Reflection->RegisterFieldSize(
				2,
				decaproto::MsgCast(&UdongConfig::analog_switch_groups_size));
		    
			// Mutable getter for button_assignments
			kUdongConfig__Reflection->RegisterGetRepeatedMessage(
				3,
				decaproto::MsgCast(&UdongConfig::get_button_assignments));
			kUdongConfig__Reflection->RegisterAddRepeatedMessage(
				3,
				decaproto::MsgCast(&UdongConfig::add_button_assignments));
			kUdongConfig__Reflection->RegisterFieldSize(
				3,
				decaproto::MsgCast(&UdongConfig::button_assignments_size));
		    return kUdongConfig__Reflection;
}
