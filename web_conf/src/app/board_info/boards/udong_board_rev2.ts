import { SwitchId } from '../../../proto/config';
import { AnalogSwitch, BoardInfo, DigitalSwitch, SwitchInfo } from '../board_info';

const analog_switch_positions: [number, number][] = [
    [64, 56],  // Buton 3
    [94, 58],  // L1
    [122, 76],  // R1
    [34, 58],  // Button 2

    [122, 106],   // R2
    [34, 90],    // Button 0
    [94, 90],    // L2
    [64, 88],    // Button 1

    [-126, 102],     // L3
    [20, 128],   // R3
    [6, 72],  // Select
    [-70, 50],     // U

    [-10, 138],   // U (thumb)
    [-100, 78],     // L
    [-40, 78],    // R
    [-70, 76],     // D
];

const digital_switch_positions: [number, number][] = [
    [122, 26],  // Start
    [94, 26],  // Home
];

const analog_switch_infos: SwitchInfo[] =
    analog_switch_positions.map((pos, i) => {
        return {
            switch_id: AnalogSwitch(i),
            x: pos[0],
            y: pos[1]
        };
    });

const digital_switch_infos: SwitchInfo[] =
    digital_switch_positions.map((pos, i) => {
        return {
            switch_id: DigitalSwitch(i),
            x: pos[0],
            y: pos[1],
        };
    });

export const UdongBoardRev2Info: BoardInfo = {
    top: 0,
    left: -170,
    width: 340,
    height: 220,
    switches: [
        ...analog_switch_infos,
        ...digital_switch_infos,
    ]
};
