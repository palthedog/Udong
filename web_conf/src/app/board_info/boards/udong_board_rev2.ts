import { SwitchId } from '../../../proto/config';
import { AnalogSwitch, BoardInfo, DigitalSwitch, SwitchInfo } from '../board_info';

const analog_switch_positions: [number, number][] = [

    // -160, +7.5

    [65, 65],  // Buton 3
    [95, 70],  // L1
    [125, 75],  // R1
    [35, 70],  // Button 2

    [125, 107.5],   // R2
    [35, 102.5],    // Button 0
    [95, 102.5],    // L2
    [65, 97.5],    // Button 1

    [-125, 102.5],     // L3
    [20, 127.5],   // R3
    [5, 80],  // Select
    [-70, 42],     // U

    [-10, 137.5],   // U (thumb)
    [-100, 77.5],     // L
    [-40, 77.5],    // R
    [-70, 67.5],     // D
];

const digital_switch_positions: [number, number][] = [
    [-132.75, 30],  // Start
    [-110, 30],  // Home
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
