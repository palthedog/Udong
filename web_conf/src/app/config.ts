// TODO: Is there any chance to use ProtoBuf on Rasberry Pi????

export interface UdongConfig {
    analog_switch_assignments: Array<AnalogSwitchAssignment>,

    analog_switch_groups: Array<AnalogSwitchGroup>,

    button_assignments: Array<ButtonAssignment>,
}

export function SwitchIdToGroupId(config: UdongConfig, switch_id: number) {
    for (let key in config.analog_switch_assignments) {
        let assignment = config.analog_switch_assignments[key];
        if (switch_id == assignment.analog_switch_id) {
            return assignment.analog_switch_group_id;
        }
    }
    return 0;
}

export function SwitchIdToButtonId(config: UdongConfig, switch_id: number): ButtonId {
    for (let key in config.button_assignments) {
        let assignment = config.button_assignments[key];
        if (switch_id == assignment.switch_id) {
            return assignment.button_id;
        }
    }
    return { type: 'push', push_button: { push_button_id: 0 } };
}

export interface AnalogSwitchAssignment {
    analog_switch_id: number,
    analog_switch_group_id: number,
}

export interface AnalogSwitchGroup {
    analog_switch_group_id: number,

    trigger_type: string,

    // One (and only one) of *_trigger would be not undefined
    rapid_trigger?: RapidTrigger,
    static_trigger?: StaticTrigger,
}

export interface RapidTrigger {
    act: number;
    rel: number;
    f_act: number;
    f_rel: number;
}

export interface StaticTrigger {
    act: number;
    rel: number;
}

export interface PushButtonSelector {
    push_button_id: number,
}

export interface DPadButtonSelector {
    direction: string,
}

export interface ButtonId {
    type: string,
    push_button?: PushButtonSelector,
    d_pad?: DPadButtonSelector,
}

export function compareButtonIds(a: ButtonId, b: ButtonId): boolean {
    if (a.type != b.type) {
        return false;
    }

    if (a.type == 'push') {
        return a.push_button?.push_button_id == b.push_button?.push_button_id;
    } else if (a.type == 'd-pad') {
        return a.d_pad?.direction == b.d_pad?.direction;
    }
    return false;
}

export interface ButtonAssignment {
    switch_id: number,
    button_id: ButtonId,
}
