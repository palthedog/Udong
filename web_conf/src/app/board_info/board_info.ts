import { Injectable } from "@angular/core";
import { ButtonId, ButtonType, DPadButtonSelector, PushButtonSelector } from '../../proto/config';
import { SwitchId, SwitchType } from '../../proto/config';

import { UdongBoardRev1Info } from './boards/udong_board_rev1';
import { UdongBoardRev2Info } from './boards/udong_board_rev2';

export function AnalogSwitch(analog_switch_id: number): SwitchId {
    return new SwitchId({
        type: SwitchType.ANALOG_SWITCH,
        id: analog_switch_id,
    });
};

export function DigitalSwitch(digital_switch_id: number): SwitchId {
    return new SwitchId({
        type: SwitchType.DIGITAL_SWITCH,
        id: digital_switch_id,
    });
};

export interface SwitchInfo {
    switch_id: SwitchId;
    x: number;
    y: number;
};

export interface BoardInfo {
    top: number;
    left: number;
    width: number;
    height: number;
    switches: SwitchInfo[];
}

@Injectable(
    { providedIn: 'root' }
)
export class BoardInfoProvider {
    boards: Map<string, BoardInfo> = new Map([
        ['udong_board_rev2', UdongBoardRev2Info],
        ['udong_board_rev1', UdongBoardRev1Info],
    ]);

    getAllBoardNames(): string[] {
        return Array.from(this.boards.keys());
    }

    get(board_name: string): BoardInfo | undefined {
        return this.boards.get(board_name);
    }
};

