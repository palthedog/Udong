import { Injectable } from "@angular/core";
import { ButtonId, ButtonType, DPadButtonSelector, PushButtonSelector } from '../proto/config';

@Injectable(
    { providedIn: 'root' }
)
export class AppConsts {
    _button_ids: ButtonId[] = [];

    get button_ids(): ButtonId[] {
        if (this._button_ids.length != 0) {
            return this._button_ids;
        }
        for (let i = 0; i < 16; i++) {
            this._button_ids.push(new ButtonId({
                type: ButtonType.PUSH,
                push_button: new PushButtonSelector({
                    push_button_id: i,
                })
            }));
        }
        this._button_ids.push(new ButtonId({
            type: ButtonType.D_PAD,
            d_pad: new DPadButtonSelector({ direction: DPadButtonSelector.Direction.UP })
        }));
        this._button_ids.push(new ButtonId({
            type: ButtonType.D_PAD,
            d_pad: new DPadButtonSelector({ direction: DPadButtonSelector.Direction.RIGHT })
        }));
        this._button_ids.push(new ButtonId({
            type: ButtonType.D_PAD,
            d_pad: new DPadButtonSelector({ direction: DPadButtonSelector.Direction.DOWN })
        }));
        this._button_ids.push(new ButtonId({
            type: ButtonType.D_PAD,
            d_pad: new DPadButtonSelector({ direction: DPadButtonSelector.Direction.LEFT })
        }));

        return this._button_ids;
    }
    /*
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
    */

    buttonName(button_id: ButtonId): string {
        switch (button_id.type) {
            case ButtonType.PUSH:
                return 'B' + button_id.push_button?.push_button_id;
            case ButtonType.D_PAD:
                switch (button_id.d_pad?.direction) {
                    case DPadButtonSelector.Direction.UP:
                        return '↑';
                    case DPadButtonSelector.Direction.RIGHT:
                        return '→';
                    case DPadButtonSelector.Direction.DOWN:
                        return '↓';
                    case DPadButtonSelector.Direction.LEFT:
                        return '←';
                }
        }
        return "??";
    }

    configGroupColor(group_id: number) {
        let hue = group_id * (360.0 / 8);
        return 'hsl(' + hue + ' 80% 60%)';
    }
}