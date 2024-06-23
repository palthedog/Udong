// TODO: Is there any chance to use ProtoBuf on Rasberry Pi????

export interface UdongConfig {
    analog_switch_assignments: Array<AnalogSwitchAssignment>,

    analog_switch_groups: Array<AnalogSwitchGroup>,
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