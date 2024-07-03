#pragma once

#include "decaproto/message.h"
#include "decaproto/descriptor.h"
#include "decaproto/reflection.h"
#include "decaproto/field.h"
#include <memory>
#include <stdint.h>
#include <string>
#include <vector>

class AnalogSwitchAssignment;
class AnalogSwitchGroup;
class RapidTriggerConfig;
class StaticTriggerConfig;
class ButtonId;
class PushButtonSelector;
class DPadButtonSelector;
class ButtonAssignment;
class UdongConfig;

enum TriggerType : int {
    UNSPECIFIED_TRIGGER = 0,
    RAPID_TRIGGER = 1,
    STATIC_TRIGGER = 2,
};

enum ButtonType : int {
    UNSPECIFIED_BUTTON_TYPE = 0,
    PUSH = 1,
    D_PAD = 2,
};

enum DPadButtonSelector_Direction : int {
    UNSPECIFIED_DIRECTION = 0,
    UP = 1,
    DOWN = 2,
    LEFT = 3,
    RIGHT = 4,
};


class AnalogSwitchAssignment final : public decaproto::Message {
public:
    AnalogSwitchAssignment()
        : analog_switch_id__(uint32_t())
        , analog_switch_group_id__(uint32_t()) {}

    ~AnalogSwitchAssignment() {}


	inline uint32_t analog_switch_id() const {
	    return analog_switch_id__;
	}

	inline void set_analog_switch_id( uint32_t value) {
	    analog_switch_id__ = value;
	}

	inline void clear_analog_switch_id() {
	    analog_switch_id__ = uint32_t();
	}

	inline uint32_t analog_switch_group_id() const {
	    return analog_switch_group_id__;
	}

	inline void set_analog_switch_group_id( uint32_t value) {
	    analog_switch_group_id__ = value;
	}

	inline void clear_analog_switch_group_id() {
	    analog_switch_group_id__ = uint32_t();
	}
    const decaproto::Descriptor* GetDescriptor() const override;
    const decaproto::Reflection* GetReflection() const override;

private:
    uint32_t analog_switch_id__;
    uint32_t analog_switch_group_id__;


};

class AnalogSwitchGroup final : public decaproto::Message {
public:
    AnalogSwitchGroup()
        : analog_switch_group_id__(uint32_t())
        , trigger_type__(::TriggerType())
        , rapid_trigger__()
        , has_rapid_trigger__(false)
        , static_trigger__()
        , has_static_trigger__(false) {}

    ~AnalogSwitchGroup() {}


	inline uint32_t analog_switch_group_id() const {
	    return analog_switch_group_id__;
	}

	inline void set_analog_switch_group_id( uint32_t value) {
	    analog_switch_group_id__ = value;
	}

	inline void clear_analog_switch_group_id() {
	    analog_switch_group_id__ = uint32_t();
	}

	inline ::TriggerType trigger_type() const {
	    return trigger_type__;
	}

	inline void set_trigger_type( ::TriggerType value) {
	    trigger_type__ = value;
	}

	inline void clear_trigger_type() {
	    trigger_type__ = ::TriggerType();
	}

	// Getter for rapid_trigger
	const ::RapidTriggerConfig& rapid_trigger() const {
		if (!rapid_trigger__) {
			//rapid_trigger__.reset(new ::RapidTriggerConfig());
			rapid_trigger__.resetDefault();
        }
	    return *rapid_trigger__;
	}

	// Mutable Getter for rapid_trigger
	::RapidTriggerConfig* mutable_rapid_trigger() {
	    if (!rapid_trigger__) {
			//rapid_trigger__.reset(new ::RapidTriggerConfig());
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
	const ::StaticTriggerConfig& static_trigger() const {
		if (!static_trigger__) {
			//static_trigger__.reset(new ::StaticTriggerConfig());
			static_trigger__.resetDefault();
        }
	    return *static_trigger__;
	}

	// Mutable Getter for static_trigger
	::StaticTriggerConfig* mutable_static_trigger() {
	    if (!static_trigger__) {
			//static_trigger__.reset(new ::StaticTriggerConfig());
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
    uint32_t analog_switch_group_id__;
    ::TriggerType trigger_type__;
    mutable decaproto::SubMessagePtr<::RapidTriggerConfig> rapid_trigger__;
    bool has_rapid_trigger__;
    mutable decaproto::SubMessagePtr<::StaticTriggerConfig> static_trigger__;
    bool has_static_trigger__;


};

class RapidTriggerConfig final : public decaproto::Message {
public:
    RapidTriggerConfig()
        : act__(double())
        , rel__(double())
        , f_act__(double())
        , f_rel__(double()) {}

    ~RapidTriggerConfig() {}


	inline double act() const {
	    return act__;
	}

	inline void set_act( double value) {
	    act__ = value;
	}

	inline void clear_act() {
	    act__ = double();
	}

	inline double rel() const {
	    return rel__;
	}

	inline void set_rel( double value) {
	    rel__ = value;
	}

	inline void clear_rel() {
	    rel__ = double();
	}

	inline double f_act() const {
	    return f_act__;
	}

	inline void set_f_act( double value) {
	    f_act__ = value;
	}

	inline void clear_f_act() {
	    f_act__ = double();
	}

	inline double f_rel() const {
	    return f_rel__;
	}

	inline void set_f_rel( double value) {
	    f_rel__ = value;
	}

	inline void clear_f_rel() {
	    f_rel__ = double();
	}
    const decaproto::Descriptor* GetDescriptor() const override;
    const decaproto::Reflection* GetReflection() const override;

private:
    double act__;
    double rel__;
    double f_act__;
    double f_rel__;


};

class StaticTriggerConfig final : public decaproto::Message {
public:
    StaticTriggerConfig()
        : act__(double())
        , rel__(double()) {}

    ~StaticTriggerConfig() {}


	inline double act() const {
	    return act__;
	}

	inline void set_act( double value) {
	    act__ = value;
	}

	inline void clear_act() {
	    act__ = double();
	}

	inline double rel() const {
	    return rel__;
	}

	inline void set_rel( double value) {
	    rel__ = value;
	}

	inline void clear_rel() {
	    rel__ = double();
	}
    const decaproto::Descriptor* GetDescriptor() const override;
    const decaproto::Reflection* GetReflection() const override;

private:
    double act__;
    double rel__;


};

class ButtonId final : public decaproto::Message {
public:
    ButtonId()
        : type__(::ButtonType())
        , push_button__()
        , has_push_button__(false)
        , d_pad__()
        , has_d_pad__(false) {}

    ~ButtonId() {}


	inline ::ButtonType type() const {
	    return type__;
	}

	inline void set_type( ::ButtonType value) {
	    type__ = value;
	}

	inline void clear_type() {
	    type__ = ::ButtonType();
	}

	// Getter for push_button
	const ::PushButtonSelector& push_button() const {
		if (!push_button__) {
			//push_button__.reset(new ::PushButtonSelector());
			push_button__.resetDefault();
        }
	    return *push_button__;
	}

	// Mutable Getter for push_button
	::PushButtonSelector* mutable_push_button() {
	    if (!push_button__) {
			//push_button__.reset(new ::PushButtonSelector());
			push_button__.resetDefault();
        }
        has_push_button__ = true;
	    return push_button__.get();
	}

	// Hazzer for push_button
	bool has_push_button() const {
	    return has_push_button__;
	}

	// Clearer for push_button
	void clear_push_button() {
	    push_button__.reset();
		has_push_button__ = false;
	}


	// Getter for d_pad
	const ::DPadButtonSelector& d_pad() const {
		if (!d_pad__) {
			//d_pad__.reset(new ::DPadButtonSelector());
			d_pad__.resetDefault();
        }
	    return *d_pad__;
	}

	// Mutable Getter for d_pad
	::DPadButtonSelector* mutable_d_pad() {
	    if (!d_pad__) {
			//d_pad__.reset(new ::DPadButtonSelector());
			d_pad__.resetDefault();
        }
        has_d_pad__ = true;
	    return d_pad__.get();
	}

	// Hazzer for d_pad
	bool has_d_pad() const {
	    return has_d_pad__;
	}

	// Clearer for d_pad
	void clear_d_pad() {
	    d_pad__.reset();
		has_d_pad__ = false;
	}

    const decaproto::Descriptor* GetDescriptor() const override;
    const decaproto::Reflection* GetReflection() const override;

private:
    ::ButtonType type__;
    mutable decaproto::SubMessagePtr<::PushButtonSelector> push_button__;
    bool has_push_button__;
    mutable decaproto::SubMessagePtr<::DPadButtonSelector> d_pad__;
    bool has_d_pad__;


};

class PushButtonSelector final : public decaproto::Message {
public:
    PushButtonSelector()
        : push_button_id__(uint32_t()) {}

    ~PushButtonSelector() {}


	inline uint32_t push_button_id() const {
	    return push_button_id__;
	}

	inline void set_push_button_id( uint32_t value) {
	    push_button_id__ = value;
	}

	inline void clear_push_button_id() {
	    push_button_id__ = uint32_t();
	}
    const decaproto::Descriptor* GetDescriptor() const override;
    const decaproto::Reflection* GetReflection() const override;

private:
    uint32_t push_button_id__;


};

class DPadButtonSelector final : public decaproto::Message {
public:
    DPadButtonSelector()
        : direction__(::DPadButtonSelector::Direction()) {}

    ~DPadButtonSelector() {}

    typedef DPadButtonSelector_Direction Direction;

	inline ::DPadButtonSelector::Direction direction() const {
	    return direction__;
	}

	inline void set_direction( ::DPadButtonSelector::Direction value) {
	    direction__ = value;
	}

	inline void clear_direction() {
	    direction__ = ::DPadButtonSelector::Direction();
	}
    const decaproto::Descriptor* GetDescriptor() const override;
    const decaproto::Reflection* GetReflection() const override;

private:
    ::DPadButtonSelector::Direction direction__;


};

class ButtonAssignment final : public decaproto::Message {
public:
    ButtonAssignment()
        : switch_id__(uint32_t())
        , button_id__()
        , has_button_id__(false) {}

    ~ButtonAssignment() {}


	inline uint32_t switch_id() const {
	    return switch_id__;
	}

	inline void set_switch_id( uint32_t value) {
	    switch_id__ = value;
	}

	inline void clear_switch_id() {
	    switch_id__ = uint32_t();
	}

	// Getter for button_id
	const ::ButtonId& button_id() const {
		if (!button_id__) {
			//button_id__.reset(new ::ButtonId());
			button_id__.resetDefault();
        }
	    return *button_id__;
	}

	// Mutable Getter for button_id
	::ButtonId* mutable_button_id() {
	    if (!button_id__) {
			//button_id__.reset(new ::ButtonId());
			button_id__.resetDefault();
        }
        has_button_id__ = true;
	    return button_id__.get();
	}

	// Hazzer for button_id
	bool has_button_id() const {
	    return has_button_id__;
	}

	// Clearer for button_id
	void clear_button_id() {
	    button_id__.reset();
		has_button_id__ = false;
	}

    const decaproto::Descriptor* GetDescriptor() const override;
    const decaproto::Reflection* GetReflection() const override;

private:
    uint32_t switch_id__;
    mutable decaproto::SubMessagePtr<::ButtonId> button_id__;
    bool has_button_id__;


};

class UdongConfig final : public decaproto::Message {
public:
    UdongConfig()
        : analog_switch_assignments__()
        , analog_switch_groups__()
        , button_assignments__() {}

    ~UdongConfig() {}


	inline const std::vector<::AnalogSwitchAssignment>& analog_switch_assignments() const {
	    return analog_switch_assignments__;
	}

	inline const ::AnalogSwitchAssignment& get_analog_switch_assignments(size_t index) const {
	    return analog_switch_assignments__[index];
	}

	inline size_t analog_switch_assignments_size() const {
	    return analog_switch_assignments__.size();
	}

	inline void set_analog_switch_assignments(size_t index, const ::AnalogSwitchAssignment& value) {
	    analog_switch_assignments__[index] = value;
	}

	inline std::vector<::AnalogSwitchAssignment>* mutable_analog_switch_assignments() {
		return &analog_switch_assignments__;
	}

	inline ::AnalogSwitchAssignment* add_analog_switch_assignments() {
	    analog_switch_assignments__.push_back(::AnalogSwitchAssignment());
		return &analog_switch_assignments__.back();
	}

	inline void clear_analog_switch_assignments() {
	    analog_switch_assignments__.clear();
	}


	inline const std::vector<::AnalogSwitchGroup>& analog_switch_groups() const {
	    return analog_switch_groups__;
	}

	inline const ::AnalogSwitchGroup& get_analog_switch_groups(size_t index) const {
	    return analog_switch_groups__[index];
	}

	inline size_t analog_switch_groups_size() const {
	    return analog_switch_groups__.size();
	}

	inline void set_analog_switch_groups(size_t index, const ::AnalogSwitchGroup& value) {
	    analog_switch_groups__[index] = value;
	}

	inline std::vector<::AnalogSwitchGroup>* mutable_analog_switch_groups() {
		return &analog_switch_groups__;
	}

	inline ::AnalogSwitchGroup* add_analog_switch_groups() {
	    analog_switch_groups__.push_back(::AnalogSwitchGroup());
		return &analog_switch_groups__.back();
	}

	inline void clear_analog_switch_groups() {
	    analog_switch_groups__.clear();
	}


	inline const std::vector<::ButtonAssignment>& button_assignments() const {
	    return button_assignments__;
	}

	inline const ::ButtonAssignment& get_button_assignments(size_t index) const {
	    return button_assignments__[index];
	}

	inline size_t button_assignments_size() const {
	    return button_assignments__.size();
	}

	inline void set_button_assignments(size_t index, const ::ButtonAssignment& value) {
	    button_assignments__[index] = value;
	}

	inline std::vector<::ButtonAssignment>* mutable_button_assignments() {
		return &button_assignments__;
	}

	inline ::ButtonAssignment* add_button_assignments() {
	    button_assignments__.push_back(::ButtonAssignment());
		return &button_assignments__.back();
	}

	inline void clear_button_assignments() {
	    button_assignments__.clear();
	}

    const decaproto::Descriptor* GetDescriptor() const override;
    const decaproto::Reflection* GetReflection() const override;

private:
    std::vector<::AnalogSwitchAssignment> analog_switch_assignments__;
    std::vector<::AnalogSwitchGroup> analog_switch_groups__;
    std::vector<::ButtonAssignment> button_assignments__;


};


/*
name: "config.proto"
message_type: {
    name: "AnalogSwitchAssignment"
    field: {
        name: "analog_switch_id"
        number: 1
        label: LABEL_OPTIONAL
        type: TYPE_UINT32
        json_name: "analogSwitchId"
    }
    field: {
        name: "analog_switch_group_id"
        number: 2
        label: LABEL_OPTIONAL
        type: TYPE_UINT32
        json_name: "analogSwitchGroupId"
    }
}
message_type: {
    name: "AnalogSwitchGroup"
    field: {
        name: "analog_switch_group_id"
        number: 1
        label: LABEL_OPTIONAL
        type: TYPE_UINT32
        json_name: "analogSwitchGroupId"
    }
    field: {
        name: "trigger_type"
        number: 2
        label: LABEL_OPTIONAL
        type: TYPE_ENUM
        type_name: ".TriggerType"
        json_name: "triggerType"
    }
    field: {
        name: "rapid_trigger"
        number: 3
        label: LABEL_OPTIONAL
        type: TYPE_MESSAGE
        type_name: ".RapidTriggerConfig"
        json_name: "rapidTrigger"
    }
    field: {
        name: "static_trigger"
        number: 4
        label: LABEL_OPTIONAL
        type: TYPE_MESSAGE
        type_name: ".StaticTriggerConfig"
        json_name: "staticTrigger"
    }
}
message_type: {
    name: "RapidTriggerConfig"
    field: {
        name: "act"
        number: 1
        label: LABEL_OPTIONAL
        type: TYPE_DOUBLE
        json_name: "act"
    }
    field: {
        name: "rel"
        number: 2
        label: LABEL_OPTIONAL
        type: TYPE_DOUBLE
        json_name: "rel"
    }
    field: {
        name: "f_act"
        number: 3
        label: LABEL_OPTIONAL
        type: TYPE_DOUBLE
        json_name: "fAct"
    }
    field: {
        name: "f_rel"
        number: 4
        label: LABEL_OPTIONAL
        type: TYPE_DOUBLE
        json_name: "fRel"
    }
}
message_type: {
    name: "StaticTriggerConfig"
    field: {
        name: "act"
        number: 1
        label: LABEL_OPTIONAL
        type: TYPE_DOUBLE
        json_name: "act"
    }
    field: {
        name: "rel"
        number: 2
        label: LABEL_OPTIONAL
        type: TYPE_DOUBLE
        json_name: "rel"
    }
}
message_type: {
    name: "ButtonId"
    field: {
        name: "type"
        number: 1
        label: LABEL_OPTIONAL
        type: TYPE_ENUM
        type_name: ".ButtonType"
        json_name: "type"
    }
    field: {
        name: "push_button"
        number: 2
        label: LABEL_OPTIONAL
        type: TYPE_MESSAGE
        type_name: ".PushButtonSelector"
        oneof_index: 0
        json_name: "pushButton"
    }
    field: {
        name: "d_pad"
        number: 3
        label: LABEL_OPTIONAL
        type: TYPE_MESSAGE
        type_name: ".DPadButtonSelector"
        oneof_index: 0
        json_name: "dPad"
    }
    oneof_decl: {
        name: "selector"
    }
}
message_type: {
    name: "PushButtonSelector"
    field: {
        name: "push_button_id"
        number: 1
        label: LABEL_OPTIONAL
        type: TYPE_UINT32
        json_name: "pushButtonId"
    }
}
message_type: {
    name: "DPadButtonSelector"
    field: {
        name: "direction"
        number: 1
        label: LABEL_OPTIONAL
        type: TYPE_ENUM
        type_name: ".DPadButtonSelector.Direction"
        json_name: "direction"
    }
    enum_type: {
        name: "Direction"
        value: {
            name: "UNSPECIFIED_DIRECTION"
            number: 0
        }
        value: {
            name: "UP"
            number: 1
        }
        value: {
            name: "DOWN"
            number: 2
        }
        value: {
            name: "LEFT"
            number: 3
        }
        value: {
            name: "RIGHT"
            number: 4
        }
    }
}
message_type: {
    name: "ButtonAssignment"
    field: {
        name: "switch_id"
        number: 1
        label: LABEL_OPTIONAL
        type: TYPE_UINT32
        json_name: "switchId"
    }
    field: {
        name: "button_id"
        number: 2
        label: LABEL_OPTIONAL
        type: TYPE_MESSAGE
        type_name: ".ButtonId"
        json_name: "buttonId"
    }
}
message_type: {
    name: "UdongConfig"
    field: {
        name: "analog_switch_assignments"
        number: 1
        label: LABEL_REPEATED
        type: TYPE_MESSAGE
        type_name: ".AnalogSwitchAssignment"
        json_name: "analogSwitchAssignments"
    }
    field: {
        name: "analog_switch_groups"
        number: 2
        label: LABEL_REPEATED
        type: TYPE_MESSAGE
        type_name: ".AnalogSwitchGroup"
        json_name: "analogSwitchGroups"
    }
    field: {
        name: "button_assignments"
        number: 3
        label: LABEL_REPEATED
        type: TYPE_MESSAGE
        type_name: ".ButtonAssignment"
        json_name: "buttonAssignments"
    }
}
enum_type: {
    name: "TriggerType"
    value: {
        name: "UNSPECIFIED_TRIGGER"
        number: 0
    }
    value: {
        name: "RAPID_TRIGGER"
        number: 1
    }
    value: {
        name: "STATIC_TRIGGER"
        number: 2
    }
}
enum_type: {
    name: "ButtonType"
    value: {
        name: "UNSPECIFIED_BUTTON_TYPE"
        number: 0
    }
    value: {
        name: "PUSH"
        number: 1
    }
    value: {
        name: "D_PAD"
        number: 2
    }
}
source_code_info: {
    location: {
        span: 0
        span: 0
        span: 87
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
        span: 4
        span: 0
        span: 7
        span: 1
        leading_comments: " Maps an analog switch to a group\n Switches in the same group share the same configuration\n"
    }
    location: {
        path: 4
        path: 0
        path: 1
        span: 4
        span: 8
        span: 30
    }
    location: {
        path: 4
        path: 0
        path: 2
        path: 0
        span: 5
        span: 2
        span: 30
    }
    location: {
        path: 4
        path: 0
        path: 2
        path: 0
        path: 5
        span: 5
        span: 2
        span: 8
    }
    location: {
        path: 4
        path: 0
        path: 2
        path: 0
        path: 1
        span: 5
        span: 9
        span: 25
    }
    location: {
        path: 4
        path: 0
        path: 2
        path: 0
        path: 3
        span: 5
        span: 28
        span: 29
    }
    location: {
        path: 4
        path: 0
        path: 2
        path: 1
        span: 6
        span: 2
        span: 36
    }
    location: {
        path: 4
        path: 0
        path: 2
        path: 1
        path: 5
        span: 6
        span: 2
        span: 8
    }
    location: {
        path: 4
        path: 0
        path: 2
        path: 1
        path: 1
        span: 6
        span: 9
        span: 31
    }
    location: {
        path: 4
        path: 0
        path: 2
        path: 1
        path: 3
        span: 6
        span: 34
        span: 35
    }
    location: {
        path: 5
        path: 0
        span: 9
        span: 0
        span: 13
        span: 1
    }
    location: {
        path: 5
        path: 0
        path: 1
        span: 9
        span: 5
        span: 16
    }
    location: {
        path: 5
        path: 0
        path: 2
        path: 0
        span: 10
        span: 2
        span: 26
    }
    location: {
        path: 5
        path: 0
        path: 2
        path: 0
        path: 1
        span: 10
        span: 2
        span: 21
    }
    location: {
        path: 5
        path: 0
        path: 2
        path: 0
        path: 2
        span: 10
        span: 24
        span: 25
    }
    location: {
        path: 5
        path: 0
        path: 2
        path: 1
        span: 11
        span: 2
        span: 20
    }
    location: {
        path: 5
        path: 0
        path: 2
        path: 1
        path: 1
        span: 11
        span: 2
        span: 15
    }
    location: {
        path: 5
        path: 0
        path: 2
        path: 1
        path: 2
        span: 11
        span: 18
        span: 19
    }
    location: {
        path: 5
        path: 0
        path: 2
        path: 2
        span: 12
        span: 2
        span: 21
    }
    location: {
        path: 5
        path: 0
        path: 2
        path: 2
        path: 1
        span: 12
        span: 2
        span: 16
    }
    location: {
        path: 5
        path: 0
        path: 2
        path: 2
        path: 2
        span: 12
        span: 19
        span: 20
    }
    location: {
        path: 4
        path: 1
        span: 16
        span: 0
        span: 25
        span: 1
        leading_comments: " Configurations for a group of analog switches\n"
    }
    location: {
        path: 4
        path: 1
        path: 1
        span: 16
        span: 8
        span: 25
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 0
        span: 18
        span: 2
        span: 36
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 0
        path: 5
        span: 18
        span: 2
        span: 8
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 0
        path: 1
        span: 18
        span: 9
        span: 31
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 0
        path: 3
        span: 18
        span: 34
        span: 35
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 1
        span: 20
        span: 2
        span: 31
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 1
        path: 6
        span: 20
        span: 2
        span: 13
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 1
        path: 1
        span: 20
        span: 14
        span: 26
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 1
        path: 3
        span: 20
        span: 29
        span: 30
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 2
        span: 23
        span: 2
        span: 39
        leading_comments: " Do NOT use oneof here so that we can keep the configuration for both trigger types\n"
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 2
        path: 6
        span: 23
        span: 2
        span: 20
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 2
        path: 1
        span: 23
        span: 21
        span: 34
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 2
        path: 3
        span: 23
        span: 37
        span: 38
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 3
        span: 24
        span: 2
        span: 41
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 3
        path: 6
        span: 24
        span: 2
        span: 21
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 3
        path: 1
        span: 24
        span: 22
        span: 36
    }
    location: {
        path: 4
        path: 1
        path: 2
        path: 3
        path: 3
        span: 24
        span: 39
        span: 40
    }
    location: {
        path: 4
        path: 2
        span: 28
        span: 0
        span: 33
        span: 1
        leading_comments: " Represents a rapid trigger configuration\n"
    }
    location: {
        path: 4
        path: 2
        path: 1
        span: 28
        span: 8
        span: 26
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 0
        span: 29
        span: 2
        span: 17
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 0
        path: 5
        span: 29
        span: 2
        span: 8
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 0
        path: 1
        span: 29
        span: 9
        span: 12
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 0
        path: 3
        span: 29
        span: 15
        span: 16
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 1
        span: 30
        span: 2
        span: 17
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 1
        path: 5
        span: 30
        span: 2
        span: 8
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 1
        path: 1
        span: 30
        span: 9
        span: 12
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 1
        path: 3
        span: 30
        span: 15
        span: 16
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 2
        span: 31
        span: 2
        span: 19
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 2
        path: 5
        span: 31
        span: 2
        span: 8
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 2
        path: 1
        span: 31
        span: 9
        span: 14
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 2
        path: 3
        span: 31
        span: 17
        span: 18
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 3
        span: 32
        span: 2
        span: 19
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 3
        path: 5
        span: 32
        span: 2
        span: 8
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 3
        path: 1
        span: 32
        span: 9
        span: 14
    }
    location: {
        path: 4
        path: 2
        path: 2
        path: 3
        path: 3
        span: 32
        span: 17
        span: 18
    }
    location: {
        path: 4
        path: 3
        span: 36
        span: 0
        span: 39
        span: 1
        leading_comments: " Represents a static trigger configuration\n"
    }
    location: {
        path: 4
        path: 3
        path: 1
        span: 36
        span: 8
        span: 27
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 0
        span: 37
        span: 2
        span: 17
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 0
        path: 5
        span: 37
        span: 2
        span: 8
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 0
        path: 1
        span: 37
        span: 9
        span: 12
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 0
        path: 3
        span: 37
        span: 15
        span: 16
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 1
        span: 38
        span: 2
        span: 17
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 1
        path: 5
        span: 38
        span: 2
        span: 8
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 1
        path: 1
        span: 38
        span: 9
        span: 12
    }
    location: {
        path: 4
        path: 3
        path: 2
        path: 1
        path: 3
        span: 38
        span: 15
        span: 16
    }
    location: {
        path: 5
        path: 1
        span: 41
        span: 0
        span: 45
        span: 1
    }
    location: {
        path: 5
        path: 1
        path: 1
        span: 41
        span: 5
        span: 15
    }
    location: {
        path: 5
        path: 1
        path: 2
        path: 0
        span: 42
        span: 2
        span: 30
    }
    location: {
        path: 5
        path: 1
        path: 2
        path: 0
        path: 1
        span: 42
        span: 2
        span: 25
    }
    location: {
        path: 5
        path: 1
        path: 2
        path: 0
        path: 2
        span: 42
        span: 28
        span: 29
    }
    location: {
        path: 5
        path: 1
        path: 2
        path: 1
        span: 43
        span: 2
        span: 11
    }
    location: {
        path: 5
        path: 1
        path: 2
        path: 1
        path: 1
        span: 43
        span: 2
        span: 6
    }
    location: {
        path: 5
        path: 1
        path: 2
        path: 1
        path: 2
        span: 43
        span: 9
        span: 10
    }
    location: {
        path: 5
        path: 1
        path: 2
        path: 2
        span: 44
        span: 2
        span: 12
    }
    location: {
        path: 5
        path: 1
        path: 2
        path: 2
        path: 1
        span: 44
        span: 2
        span: 7
    }
    location: {
        path: 5
        path: 1
        path: 2
        path: 2
        path: 2
        span: 44
        span: 10
        span: 11
    }
    location: {
        path: 4
        path: 4
        span: 48
        span: 0
        span: 56
        span: 1
        leading_comments: " A button ID used to identify a button on the USB gamepad\n"
    }
    location: {
        path: 4
        path: 4
        path: 1
        span: 48
        span: 8
        span: 16
    }
    location: {
        path: 4
        path: 4
        path: 2
        path: 0
        span: 50
        span: 2
        span: 22
    }
    location: {
        path: 4
        path: 4
        path: 2
        path: 0
        path: 6
        span: 50
        span: 2
        span: 12
    }
    location: {
        path: 4
        path: 4
        path: 2
        path: 0
        path: 1
        span: 50
        span: 13
        span: 17
    }
    location: {
        path: 4
        path: 4
        path: 2
        path: 0
        path: 3
        span: 50
        span: 20
        span: 21
    }
    location: {
        path: 4
        path: 4
        path: 8
        path: 0
        span: 52
        span: 2
        span: 55
        span: 3
    }
    location: {
        path: 4
        path: 4
        path: 8
        path: 0
        path: 1
        span: 52
        span: 8
        span: 16
    }
    location: {
        path: 4
        path: 4
        path: 2
        path: 1
        span: 53
        span: 4
        span: 39
    }
    location: {
        path: 4
        path: 4
        path: 2
        path: 1
        path: 6
        span: 53
        span: 4
        span: 22
    }
    location: {
        path: 4
        path: 4
        path: 2
        path: 1
        path: 1
        span: 53
        span: 23
        span: 34
    }
    location: {
        path: 4
        path: 4
        path: 2
        path: 1
        path: 3
        span: 53
        span: 37
        span: 38
    }
    location: {
        path: 4
        path: 4
        path: 2
        path: 2
        span: 54
        span: 4
        span: 33
    }
    location: {
        path: 4
        path: 4
        path: 2
        path: 2
        path: 6
        span: 54
        span: 4
        span: 22
    }
    location: {
        path: 4
        path: 4
        path: 2
        path: 2
        path: 1
        span: 54
        span: 23
        span: 28
    }
    location: {
        path: 4
        path: 4
        path: 2
        path: 2
        path: 3
        span: 54
        span: 31
        span: 32
    }
    location: {
        path: 4
        path: 5
        span: 59
        span: 0
        span: 61
        span: 1
        leading_comments: " An ID for push buttons\n"
    }
    location: {
        path: 4
        path: 5
        path: 1
        span: 59
        span: 8
        span: 26
    }
    location: {
        path: 4
        path: 5
        path: 2
        path: 0
        span: 60
        span: 2
        span: 28
    }
    location: {
        path: 4
        path: 5
        path: 2
        path: 0
        path: 5
        span: 60
        span: 2
        span: 8
    }
    location: {
        path: 4
        path: 5
        path: 2
        path: 0
        path: 1
        span: 60
        span: 9
        span: 23
    }
    location: {
        path: 4
        path: 5
        path: 2
        path: 0
        path: 3
        span: 60
        span: 26
        span: 27
    }
    location: {
        path: 4
        path: 6
        span: 64
        span: 0
        span: 74
        span: 1
        leading_comments: " An ID for D-pad buttons\n"
    }
    location: {
        path: 4
        path: 6
        path: 1
        span: 64
        span: 8
        span: 26
    }
    location: {
        path: 4
        path: 6
        path: 4
        path: 0
        span: 65
        span: 2
        span: 71
        span: 3
    }
    location: {
        path: 4
        path: 6
        path: 4
        path: 0
        path: 1
        span: 65
        span: 7
        span: 16
    }
    location: {
        path: 4
        path: 6
        path: 4
        path: 0
        path: 2
        path: 0
        span: 66
        span: 4
        span: 30
    }
    location: {
        path: 4
        path: 6
        path: 4
        path: 0
        path: 2
        path: 0
        path: 1
        span: 66
        span: 4
        span: 25
    }
    location: {
        path: 4
        path: 6
        path: 4
        path: 0
        path: 2
        path: 0
        path: 2
        span: 66
        span: 28
        span: 29
    }
    location: {
        path: 4
        path: 6
        path: 4
        path: 0
        path: 2
        path: 1
        span: 67
        span: 4
        span: 11
    }
    location: {
        path: 4
        path: 6
        path: 4
        path: 0
        path: 2
        path: 1
        path: 1
        span: 67
        span: 4
        span: 6
    }
    location: {
        path: 4
        path: 6
        path: 4
        path: 0
        path: 2
        path: 1
        path: 2
        span: 67
        span: 9
        span: 10
    }
    location: {
        path: 4
        path: 6
        path: 4
        path: 0
        path: 2
        path: 2
        span: 68
        span: 4
        span: 13
    }
    location: {
        path: 4
        path: 6
        path: 4
        path: 0
        path: 2
        path: 2
        path: 1
        span: 68
        span: 4
        span: 8
    }
    location: {
        path: 4
        path: 6
        path: 4
        path: 0
        path: 2
        path: 2
        path: 2
        span: 68
        span: 11
        span: 12
    }
    location: {
        path: 4
        path: 6
        path: 4
        path: 0
        path: 2
        path: 3
        span: 69
        span: 4
        span: 13
    }
    location: {
        path: 4
        path: 6
        path: 4
        path: 0
        path: 2
        path: 3
        path: 1
        span: 69
        span: 4
        span: 8
    }
    location: {
        path: 4
        path: 6
        path: 4
        path: 0
        path: 2
        path: 3
        path: 2
        span: 69
        span: 11
        span: 12
    }
    location: {
        path: 4
        path: 6
        path: 4
        path: 0
        path: 2
        path: 4
        span: 70
        span: 4
        span: 14
    }
    location: {
        path: 4
        path: 6
        path: 4
        path: 0
        path: 2
        path: 4
        path: 1
        span: 70
        span: 4
        span: 9
    }
    location: {
        path: 4
        path: 6
        path: 4
        path: 0
        path: 2
        path: 4
        path: 2
        span: 70
        span: 12
        span: 13
    }
    location: {
        path: 4
        path: 6
        path: 2
        path: 0
        span: 73
        span: 2
        span: 26
    }
    location: {
        path: 4
        path: 6
        path: 2
        path: 0
        path: 6
        span: 73
        span: 2
        span: 11
    }
    location: {
        path: 4
        path: 6
        path: 2
        path: 0
        path: 1
        span: 73
        span: 12
        span: 21
    }
    location: {
        path: 4
        path: 6
        path: 2
        path: 0
        path: 3
        span: 73
        span: 24
        span: 25
    }
    location: {
        path: 4
        path: 7
        span: 77
        span: 0
        span: 80
        span: 1
        leading_comments: " Maps a switch to a button\n"
    }
    location: {
        path: 4
        path: 7
        path: 1
        span: 77
        span: 8
        span: 24
    }
    location: {
        path: 4
        path: 7
        path: 2
        path: 0
        span: 78
        span: 2
        span: 23
    }
    location: {
        path: 4
        path: 7
        path: 2
        path: 0
        path: 5
        span: 78
        span: 2
        span: 8
    }
    location: {
        path: 4
        path: 7
        path: 2
        path: 0
        path: 1
        span: 78
        span: 9
        span: 18
    }
    location: {
        path: 4
        path: 7
        path: 2
        path: 0
        path: 3
        span: 78
        span: 21
        span: 22
    }
    location: {
        path: 4
        path: 7
        path: 2
        path: 1
        span: 79
        span: 2
        span: 25
    }
    location: {
        path: 4
        path: 7
        path: 2
        path: 1
        path: 6
        span: 79
        span: 2
        span: 10
    }
    location: {
        path: 4
        path: 7
        path: 2
        path: 1
        path: 1
        span: 79
        span: 11
        span: 20
    }
    location: {
        path: 4
        path: 7
        path: 2
        path: 1
        path: 3
        span: 79
        span: 23
        span: 24
    }
    location: {
        path: 4
        path: 8
        span: 83
        span: 0
        span: 87
        span: 1
        leading_comments: " Represents the overall configuration for Udong\n"
    }
    location: {
        path: 4
        path: 8
        path: 1
        span: 83
        span: 8
        span: 19
    }
    location: {
        path: 4
        path: 8
        path: 2
        path: 0
        span: 84
        span: 2
        span: 64
    }
    location: {
        path: 4
        path: 8
        path: 2
        path: 0
        path: 4
        span: 84
        span: 2
        span: 10
    }
    location: {
        path: 4
        path: 8
        path: 2
        path: 0
        path: 6
        span: 84
        span: 11
        span: 33
    }
    location: {
        path: 4
        path: 8
        path: 2
        path: 0
        path: 1
        span: 84
        span: 34
        span: 59
    }
    location: {
        path: 4
        path: 8
        path: 2
        path: 0
        path: 3
        span: 84
        span: 62
        span: 63
    }
    location: {
        path: 4
        path: 8
        path: 2
        path: 1
        span: 85
        span: 2
        span: 54
    }
    location: {
        path: 4
        path: 8
        path: 2
        path: 1
        path: 4
        span: 85
        span: 2
        span: 10
    }
    location: {
        path: 4
        path: 8
        path: 2
        path: 1
        path: 6
        span: 85
        span: 11
        span: 28
    }
    location: {
        path: 4
        path: 8
        path: 2
        path: 1
        path: 1
        span: 85
        span: 29
        span: 49
    }
    location: {
        path: 4
        path: 8
        path: 2
        path: 1
        path: 3
        span: 85
        span: 52
        span: 53
    }
    location: {
        path: 4
        path: 8
        path: 2
        path: 2
        span: 86
        span: 2
        span: 51
    }
    location: {
        path: 4
        path: 8
        path: 2
        path: 2
        path: 4
        span: 86
        span: 2
        span: 10
    }
    location: {
        path: 4
        path: 8
        path: 2
        path: 2
        path: 6
        span: 86
        span: 11
        span: 27
    }
    location: {
        path: 4
        path: 8
        path: 2
        path: 2
        path: 1
        span: 86
        span: 28
        span: 46
    }
    location: {
        path: 4
        path: 8
        path: 2
        path: 2
        path: 3
        span: 86
        span: 49
        span: 50
    }
}
syntax: "proto3"
*/
