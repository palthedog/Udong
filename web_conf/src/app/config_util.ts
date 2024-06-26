// TODO: Is there any chance to use ProtoBuf on Rasberry Pi????

import { ButtonAssignment, ButtonId, ButtonType, PushButtonSelector, UdongConfig } from '../proto/config';

export function SwitchIdToGroupId(config: UdongConfig, switch_id: number) {
    for (let key in config.analog_switch_assignments) {
        let assignment = config.analog_switch_assignments[key];
        if (switch_id === assignment.analog_switch_id) {
            return assignment.analog_switch_group_id;
        }
    }
    return 0;
}

export function SwitchIdToButtonId(config: UdongConfig, switch_id: number): ButtonId {
    for (let key in config.button_assignments) {
        let assignment: ButtonAssignment = config.button_assignments[key];
        if (switch_id === assignment.switch_id) {
            return assignment.button_id;
        }
    }
    return new ButtonId({
        type: ButtonType.PUSH,
        push_button: new PushButtonSelector({ push_button_id: 0 })
    });
}

export function compareButtonIds(a: ButtonId, b: ButtonId): boolean {
    if (a.type != b.type) {
        return false;
    }

    if (a.type == ButtonType.PUSH) {
        return a.push_button?.push_button_id == b.push_button?.push_button_id;
    } else if (a.type == ButtonType.D_PAD) {
        return a.d_pad?.direction == b.d_pad?.direction;
    }
    return false;
}
