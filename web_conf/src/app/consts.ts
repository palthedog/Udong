import { Injectable } from "@angular/core";
import { UdongConfig } from "./config";

@Injectable(
    { providedIn: 'root' }
)
export class AppConsts {
    button_ids: number[] = new Array(18).fill(0).map((_v, index) => index);
    button_names: string[] = [
        'B1',
        'B2',
        'B3',
        'B4',

        'L1',
        'R1',
        'L2',
        'R2',

        'S1',
        'S2',
        'L3',
        'R3',

        'Z1',
        'Z2',
        'Z3',
        'Z4',
        'Z5',
        'Z6',
    ];

    configGroupColor(group_id: number) {
        let hue = group_id * (360.0 / 8);
        return 'hsl(' + hue + ' 80% 60%)';
    }
}