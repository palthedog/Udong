#include "config.pb.h"

#include <cassert>
#include "decaproto/reflection_util.h"
#include "decaproto/encoder.h"
#include "decaproto/stream/coded_stream.h"


// A singleton Descriptor for AnalogSwitchConfig
decaproto::Descriptor* kAnalogSwitchConfig__Descriptor = nullptr;

const decaproto::Descriptor* AnalogSwitchConfig::GetDescriptor() const {
    if (kAnalogSwitchConfig__Descriptor != nullptr) {
        return kAnalogSwitchConfig__Descriptor;
    }
    kAnalogSwitchConfig__Descriptor = new decaproto::Descriptor();
    kAnalogSwitchConfig__Descriptor->RegisterField(decaproto::FieldDescriptor(1, decaproto::FieldType::kUint32));
    kAnalogSwitchConfig__Descriptor->RegisterField(decaproto::FieldDescriptor(2, decaproto::FieldType::kUint32));
    return kAnalogSwitchConfig__Descriptor;
}

// A singleton Reflection object for AnalogSwitchConfig
decaproto::Reflection* kAnalogSwitchConfig__Reflection = nullptr;

const decaproto::Reflection* AnalogSwitchConfig::GetReflection() const {
    if (kAnalogSwitchConfig__Reflection != nullptr) {
        return kAnalogSwitchConfig__Reflection;
    }
    kAnalogSwitchConfig__Reflection = new decaproto::Reflection();
    
    // Setter
    kAnalogSwitchConfig__Reflection->RegisterSetUint32(
        1,
		decaproto::MsgCast(&AnalogSwitchConfig::set_analog_switch_id));
    // Getter
    kAnalogSwitchConfig__Reflection->RegisterGetUint32(
        1,
		decaproto::MsgCast(&AnalogSwitchConfig::analog_switch_id));
        
    // Setter
    kAnalogSwitchConfig__Reflection->RegisterSetUint32(
        2,
		decaproto::MsgCast(&AnalogSwitchConfig::set_analog_switch_group_id));
    // Getter
    kAnalogSwitchConfig__Reflection->RegisterGetUint32(
        2,
		decaproto::MsgCast(&AnalogSwitchConfig::analog_switch_group_id));
        return kAnalogSwitchConfig__Reflection;
}

size_t AnalogSwitchConfig::ComputeEncodedSize() const {
    size_t size = 0;

		if ( analog_switch_id__ != uint32_t() ) {
			size += 1;  // tag
			size += decaproto::ComputeEncodedVarintSize(analog_switch_id__);
		}
		
		if ( analog_switch_group_id__ != uint32_t() ) {
			size += 1;  // tag
			size += decaproto::ComputeEncodedVarintSize(analog_switch_group_id__);
		}
				return size;
}

bool AnalogSwitchConfig::EncodeImpl(decaproto::CodedOutputStream& stream) const {

					if (analog_switch_id__ != uint32_t()) {
						stream.WriteTag(1, decaproto::WireType::kVarint);
						stream.WriteVarint32(analog_switch_id__);
					}
					
					if (analog_switch_group_id__ != uint32_t()) {
						stream.WriteTag(2, decaproto::WireType::kVarint);
						stream.WriteVarint32(analog_switch_group_id__);
					}
							return true;
}

// A singleton Descriptor for AnalogSwitchGroup
decaproto::Descriptor* kAnalogSwitchGroup__Descriptor = nullptr;

const decaproto::Descriptor* AnalogSwitchGroup::GetDescriptor() const {
    if (kAnalogSwitchGroup__Descriptor != nullptr) {
        return kAnalogSwitchGroup__Descriptor;
    }
    kAnalogSwitchGroup__Descriptor = new decaproto::Descriptor();
    kAnalogSwitchGroup__Descriptor->RegisterField(decaproto::FieldDescriptor(1, decaproto::FieldType::kUint32));
    kAnalogSwitchGroup__Descriptor->RegisterField(decaproto::FieldDescriptor(5, decaproto::FieldType::kDouble));
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
        
    // Setter
    kAnalogSwitchGroup__Reflection->RegisterSetDouble(
        5,
		decaproto::MsgCast(&AnalogSwitchGroup::set_total_travel_distance));
    // Getter
    kAnalogSwitchGroup__Reflection->RegisterGetDouble(
        5,
		decaproto::MsgCast(&AnalogSwitchGroup::total_travel_distance));
        
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

size_t AnalogSwitchGroup::ComputeEncodedSize() const {
    size_t size = 0;

		if ( analog_switch_group_id__ != uint32_t() ) {
			size += 1;  // tag
			size += decaproto::ComputeEncodedVarintSize(analog_switch_group_id__);
		}
		
		if ( total_travel_distance__ != double() ) {
			size += 1;  // tag
			size += 8;
		}
		
		if ( trigger_type__ != TriggerType() ) {
			size += 1;  // tag
			size += decaproto::ComputeEncodedVarintSize(trigger_type__);
		}
		
		if ( has_rapid_trigger() ) {
			size_t sub_msg_size = rapid_trigger__->ComputeEncodedSize();
			// tag
			size += 1;
			// LEN
			size += decaproto::ComputeEncodedVarintSize(sub_msg_size);
			// value
			size += sub_msg_size;
		}
		
		if ( has_static_trigger() ) {
			size_t sub_msg_size = static_trigger__->ComputeEncodedSize();
			// tag
			size += 1;
			// LEN
			size += decaproto::ComputeEncodedVarintSize(sub_msg_size);
			// value
			size += sub_msg_size;
		}
				return size;
}

bool AnalogSwitchGroup::EncodeImpl(decaproto::CodedOutputStream& stream) const {

					if (analog_switch_group_id__ != uint32_t()) {
						stream.WriteTag(1, decaproto::WireType::kVarint);
						stream.WriteVarint32(analog_switch_group_id__);
					}
					
					if (total_travel_distance__ != double()) {
						stream.WriteTag(5, decaproto::WireType::kI64);
						stream.WriteFixedInt64(decaproto::MemcpyCast<double, uint64_t>(total_travel_distance__));
					}
					
					if (trigger_type__ != TriggerType()) {
						stream.WriteTag(2, decaproto::WireType::kVarint);
						stream.WriteVarint32(trigger_type__);
					}
					
					if (has_rapid_trigger()) {
						size_t sub_msg_size = rapid_trigger__->ComputeEncodedSize();
						stream.WriteTag(3, decaproto::WireType::kLen);
						stream.WriteVarint32(sub_msg_size);
						rapid_trigger__->EncodeImpl(stream);
					}
					
					if (has_static_trigger()) {
						size_t sub_msg_size = static_trigger__->ComputeEncodedSize();
						stream.WriteTag(4, decaproto::WireType::kLen);
						stream.WriteVarint32(sub_msg_size);
						static_trigger__->EncodeImpl(stream);
					}
							return true;
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

size_t RapidTriggerConfig::ComputeEncodedSize() const {
    size_t size = 0;

		if ( act__ != double() ) {
			size += 1;  // tag
			size += 8;
		}
		
		if ( rel__ != double() ) {
			size += 1;  // tag
			size += 8;
		}
		
		if ( f_act__ != double() ) {
			size += 1;  // tag
			size += 8;
		}
		
		if ( f_rel__ != double() ) {
			size += 1;  // tag
			size += 8;
		}
				return size;
}

bool RapidTriggerConfig::EncodeImpl(decaproto::CodedOutputStream& stream) const {

					if (act__ != double()) {
						stream.WriteTag(1, decaproto::WireType::kI64);
						stream.WriteFixedInt64(decaproto::MemcpyCast<double, uint64_t>(act__));
					}
					
					if (rel__ != double()) {
						stream.WriteTag(2, decaproto::WireType::kI64);
						stream.WriteFixedInt64(decaproto::MemcpyCast<double, uint64_t>(rel__));
					}
					
					if (f_act__ != double()) {
						stream.WriteTag(3, decaproto::WireType::kI64);
						stream.WriteFixedInt64(decaproto::MemcpyCast<double, uint64_t>(f_act__));
					}
					
					if (f_rel__ != double()) {
						stream.WriteTag(4, decaproto::WireType::kI64);
						stream.WriteFixedInt64(decaproto::MemcpyCast<double, uint64_t>(f_rel__));
					}
							return true;
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

size_t StaticTriggerConfig::ComputeEncodedSize() const {
    size_t size = 0;

		if ( act__ != double() ) {
			size += 1;  // tag
			size += 8;
		}
		
		if ( rel__ != double() ) {
			size += 1;  // tag
			size += 8;
		}
				return size;
}

bool StaticTriggerConfig::EncodeImpl(decaproto::CodedOutputStream& stream) const {

					if (act__ != double()) {
						stream.WriteTag(1, decaproto::WireType::kI64);
						stream.WriteFixedInt64(decaproto::MemcpyCast<double, uint64_t>(act__));
					}
					
					if (rel__ != double()) {
						stream.WriteTag(2, decaproto::WireType::kI64);
						stream.WriteFixedInt64(decaproto::MemcpyCast<double, uint64_t>(rel__));
					}
							return true;
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

size_t ButtonId::ComputeEncodedSize() const {
    size_t size = 0;

		if ( type__ != ButtonType() ) {
			size += 1;  // tag
			size += decaproto::ComputeEncodedVarintSize(type__);
		}
		
		if ( has_push_button() ) {
			size_t sub_msg_size = push_button__->ComputeEncodedSize();
			// tag
			size += 1;
			// LEN
			size += decaproto::ComputeEncodedVarintSize(sub_msg_size);
			// value
			size += sub_msg_size;
		}
		
		if ( has_d_pad() ) {
			size_t sub_msg_size = d_pad__->ComputeEncodedSize();
			// tag
			size += 1;
			// LEN
			size += decaproto::ComputeEncodedVarintSize(sub_msg_size);
			// value
			size += sub_msg_size;
		}
				return size;
}

bool ButtonId::EncodeImpl(decaproto::CodedOutputStream& stream) const {

					if (type__ != ButtonType()) {
						stream.WriteTag(1, decaproto::WireType::kVarint);
						stream.WriteVarint32(type__);
					}
					
					if (has_push_button()) {
						size_t sub_msg_size = push_button__->ComputeEncodedSize();
						stream.WriteTag(2, decaproto::WireType::kLen);
						stream.WriteVarint32(sub_msg_size);
						push_button__->EncodeImpl(stream);
					}
					
					if (has_d_pad()) {
						size_t sub_msg_size = d_pad__->ComputeEncodedSize();
						stream.WriteTag(3, decaproto::WireType::kLen);
						stream.WriteVarint32(sub_msg_size);
						d_pad__->EncodeImpl(stream);
					}
							return true;
}

// A singleton Descriptor for SwitchId
decaproto::Descriptor* kSwitchId__Descriptor = nullptr;

const decaproto::Descriptor* SwitchId::GetDescriptor() const {
    if (kSwitchId__Descriptor != nullptr) {
        return kSwitchId__Descriptor;
    }
    kSwitchId__Descriptor = new decaproto::Descriptor();
    kSwitchId__Descriptor->RegisterField(decaproto::FieldDescriptor(1, decaproto::FieldType::kEnum));
    kSwitchId__Descriptor->RegisterField(decaproto::FieldDescriptor(2, decaproto::FieldType::kUint32));
    return kSwitchId__Descriptor;
}

// A singleton Reflection object for SwitchId
decaproto::Reflection* kSwitchId__Reflection = nullptr;

const decaproto::Reflection* SwitchId::GetReflection() const {
    if (kSwitchId__Reflection != nullptr) {
        return kSwitchId__Reflection;
    }
    kSwitchId__Reflection = new decaproto::Reflection();
    
    // EnumValue setter for type
     kSwitchId__Reflection->RegisterSetEnumValue(
        1,
		decaproto::CastForSetEnumValue(&SwitchId::set_type));
     // EnumValue getter for type
     kSwitchId__Reflection->RegisterGetEnumValue(
        1,
		decaproto::CastForGetEnumValue(&SwitchId::type));
    
    // Setter
    kSwitchId__Reflection->RegisterSetUint32(
        2,
		decaproto::MsgCast(&SwitchId::set_id));
    // Getter
    kSwitchId__Reflection->RegisterGetUint32(
        2,
		decaproto::MsgCast(&SwitchId::id));
        return kSwitchId__Reflection;
}

size_t SwitchId::ComputeEncodedSize() const {
    size_t size = 0;

		if ( type__ != SwitchType() ) {
			size += 1;  // tag
			size += decaproto::ComputeEncodedVarintSize(type__);
		}
		
		if ( id__ != uint32_t() ) {
			size += 1;  // tag
			size += decaproto::ComputeEncodedVarintSize(id__);
		}
				return size;
}

bool SwitchId::EncodeImpl(decaproto::CodedOutputStream& stream) const {

					if (type__ != SwitchType()) {
						stream.WriteTag(1, decaproto::WireType::kVarint);
						stream.WriteVarint32(type__);
					}
					
					if (id__ != uint32_t()) {
						stream.WriteTag(2, decaproto::WireType::kVarint);
						stream.WriteVarint32(id__);
					}
							return true;
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

size_t PushButtonSelector::ComputeEncodedSize() const {
    size_t size = 0;

		if ( push_button_id__ != uint32_t() ) {
			size += 1;  // tag
			size += decaproto::ComputeEncodedVarintSize(push_button_id__);
		}
				return size;
}

bool PushButtonSelector::EncodeImpl(decaproto::CodedOutputStream& stream) const {

					if (push_button_id__ != uint32_t()) {
						stream.WriteTag(1, decaproto::WireType::kVarint);
						stream.WriteVarint32(push_button_id__);
					}
							return true;
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

size_t DPadButtonSelector::ComputeEncodedSize() const {
    size_t size = 0;

		if ( direction__ != DPadButtonSelector_Direction() ) {
			size += 1;  // tag
			size += decaproto::ComputeEncodedVarintSize(direction__);
		}
				return size;
}

bool DPadButtonSelector::EncodeImpl(decaproto::CodedOutputStream& stream) const {

					if (direction__ != DPadButtonSelector_Direction()) {
						stream.WriteTag(1, decaproto::WireType::kVarint);
						stream.WriteVarint32(direction__);
					}
							return true;
}

// A singleton Descriptor for ButtonAssignment
decaproto::Descriptor* kButtonAssignment__Descriptor = nullptr;

const decaproto::Descriptor* ButtonAssignment::GetDescriptor() const {
    if (kButtonAssignment__Descriptor != nullptr) {
        return kButtonAssignment__Descriptor;
    }
    kButtonAssignment__Descriptor = new decaproto::Descriptor();
    kButtonAssignment__Descriptor->RegisterField(decaproto::FieldDescriptor(1, decaproto::FieldType::kMessage));
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
    
    // Mutable getter for switch_id
    kButtonAssignment__Reflection->RegisterMutableMessage(
        1,
		decaproto::MsgCast(&ButtonAssignment::mutable_switch_id));
    // Getter for switch_id
    kButtonAssignment__Reflection->RegisterGetMessage(
        1,
		decaproto::MsgCast(&ButtonAssignment::switch_id));
    // Hazzer for switch_id
    kButtonAssignment__Reflection->RegisterHasField(
        1,
		decaproto::MsgCast(&ButtonAssignment::has_switch_id));
    
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

size_t ButtonAssignment::ComputeEncodedSize() const {
    size_t size = 0;

		if ( has_switch_id() ) {
			size_t sub_msg_size = switch_id__->ComputeEncodedSize();
			// tag
			size += 1;
			// LEN
			size += decaproto::ComputeEncodedVarintSize(sub_msg_size);
			// value
			size += sub_msg_size;
		}
		
		if ( has_button_id() ) {
			size_t sub_msg_size = button_id__->ComputeEncodedSize();
			// tag
			size += 1;
			// LEN
			size += decaproto::ComputeEncodedVarintSize(sub_msg_size);
			// value
			size += sub_msg_size;
		}
				return size;
}

bool ButtonAssignment::EncodeImpl(decaproto::CodedOutputStream& stream) const {

					if (has_switch_id()) {
						size_t sub_msg_size = switch_id__->ComputeEncodedSize();
						stream.WriteTag(1, decaproto::WireType::kLen);
						stream.WriteVarint32(sub_msg_size);
						switch_id__->EncodeImpl(stream);
					}
					
					if (has_button_id()) {
						size_t sub_msg_size = button_id__->ComputeEncodedSize();
						stream.WriteTag(2, decaproto::WireType::kLen);
						stream.WriteVarint32(sub_msg_size);
						button_id__->EncodeImpl(stream);
					}
							return true;
}

// A singleton Descriptor for BakedData
decaproto::Descriptor* kBakedData__Descriptor = nullptr;

const decaproto::Descriptor* BakedData::GetDescriptor() const {
    if (kBakedData__Descriptor != nullptr) {
        return kBakedData__Descriptor;
    }
    kBakedData__Descriptor = new decaproto::Descriptor();
    kBakedData__Descriptor->RegisterField(decaproto::FieldDescriptor(1, decaproto::FieldType::kString));
    return kBakedData__Descriptor;
}

// A singleton Reflection object for BakedData
decaproto::Reflection* kBakedData__Reflection = nullptr;

const decaproto::Reflection* BakedData::GetReflection() const {
    if (kBakedData__Reflection != nullptr) {
        return kBakedData__Reflection;
    }
    kBakedData__Reflection = new decaproto::Reflection();
    
    // Mutable getter for board_name
    kBakedData__Reflection->RegisterMutableString(
        1,
		decaproto::MsgCast(&BakedData::mutable_board_name));
    // Getter for board_name
    kBakedData__Reflection->RegisterGetString(
        1,
		decaproto::MsgCast(&BakedData::board_name));
    return kBakedData__Reflection;
}

size_t BakedData::ComputeEncodedSize() const {
    size_t size = 0;

		if ( !board_name__.empty() ) {
			// tag
			size += 1;
			// LEN
			size += decaproto::ComputeEncodedVarintSize(board_name__.size());
			// value
			size += board_name__.size();
		}
				return size;
}

bool BakedData::EncodeImpl(decaproto::CodedOutputStream& stream) const {

					if (!board_name__.empty()) {
						stream.WriteTag(1, decaproto::WireType::kLen);
						stream.WriteVarint32(board_name__.size());
						stream.WriteString(board_name__);
					}
							return true;
}

// A singleton Descriptor for UdongConfig
decaproto::Descriptor* kUdongConfig__Descriptor = nullptr;

const decaproto::Descriptor* UdongConfig::GetDescriptor() const {
    if (kUdongConfig__Descriptor != nullptr) {
        return kUdongConfig__Descriptor;
    }
    kUdongConfig__Descriptor = new decaproto::Descriptor();
    kUdongConfig__Descriptor->RegisterField(decaproto::FieldDescriptor(4, decaproto::FieldType::kMessage));
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
    
    // Mutable getter for baked
    kUdongConfig__Reflection->RegisterMutableMessage(
        4,
		decaproto::MsgCast(&UdongConfig::mutable_baked));
    // Getter for baked
    kUdongConfig__Reflection->RegisterGetMessage(
        4,
		decaproto::MsgCast(&UdongConfig::baked));
    // Hazzer for baked
    kUdongConfig__Reflection->RegisterHasField(
        4,
		decaproto::MsgCast(&UdongConfig::has_baked));
    
			// Mutable getter for analog_switch_configs
			kUdongConfig__Reflection->RegisterGetRepeatedMessage(
				1,
				decaproto::MsgCast(&UdongConfig::get_analog_switch_configs));
			kUdongConfig__Reflection->RegisterAddRepeatedMessage(
				1,
				decaproto::MsgCast(&UdongConfig::add_analog_switch_configs));
			kUdongConfig__Reflection->RegisterFieldSize(
				1,
				decaproto::MsgCast(&UdongConfig::analog_switch_configs_size));
		    
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

size_t UdongConfig::ComputeEncodedSize() const {
    size_t size = 0;

		if ( has_baked() ) {
			size_t sub_msg_size = baked__->ComputeEncodedSize();
			// tag
			size += 1;
			// LEN
			size += decaproto::ComputeEncodedVarintSize(sub_msg_size);
			// value
			size += sub_msg_size;
		}
		
		for (auto& item : analog_switch_configs__) {
			size_t sub_msg_size = item.ComputeEncodedSize();
			// tag
			size += 1;
			// LEN
			size += decaproto::ComputeEncodedVarintSize(sub_msg_size);
			// value
			size += sub_msg_size;
		}
		
		for (auto& item : analog_switch_groups__) {
			size_t sub_msg_size = item.ComputeEncodedSize();
			// tag
			size += 1;
			// LEN
			size += decaproto::ComputeEncodedVarintSize(sub_msg_size);
			// value
			size += sub_msg_size;
		}
		
		for (auto& item : button_assignments__) {
			size_t sub_msg_size = item.ComputeEncodedSize();
			// tag
			size += 1;
			// LEN
			size += decaproto::ComputeEncodedVarintSize(sub_msg_size);
			// value
			size += sub_msg_size;
		}
				return size;
}

bool UdongConfig::EncodeImpl(decaproto::CodedOutputStream& stream) const {

					if (has_baked()) {
						size_t sub_msg_size = baked__->ComputeEncodedSize();
						stream.WriteTag(4, decaproto::WireType::kLen);
						stream.WriteVarint32(sub_msg_size);
						baked__->EncodeImpl(stream);
					}
					
					for (auto& item : analog_switch_configs__) {
						size_t sub_msg_size = item.ComputeEncodedSize();
						stream.WriteTag(1, decaproto::WireType::kLen);
						stream.WriteVarint32(sub_msg_size);
						item.EncodeImpl(stream);
					}
					
					for (auto& item : analog_switch_groups__) {
						size_t sub_msg_size = item.ComputeEncodedSize();
						stream.WriteTag(2, decaproto::WireType::kLen);
						stream.WriteVarint32(sub_msg_size);
						item.EncodeImpl(stream);
					}
					
					for (auto& item : button_assignments__) {
						size_t sub_msg_size = item.ComputeEncodedSize();
						stream.WriteTag(3, decaproto::WireType::kLen);
						stream.WriteVarint32(sub_msg_size);
						item.EncodeImpl(stream);
					}
							return true;
}
