import { Injectable } from '@angular/core';

import { BehaviorSubject, Observable, Subject, defer, filter } from 'rxjs';
import { SerialServiceInterface } from './serial.service';

import { AnalogSwitchConfig, AnalogSwitchGroup, ButtonAssignment, ButtonId, ButtonType, PushButtonSelector, RapidTriggerConfig, StaticTriggerConfig, SwitchId, SwitchType, TriggerType, UdongConfig } from '../../proto/config';
import { logger } from '../logger';

@Injectable()
export class MockSerialService extends SerialServiceInterface {
    private message_subject = new Subject<[string, Uint8Array]>();
    private connection_subject = new BehaviorSubject<boolean>(false);

    public ConnectionChanges(): Observable<boolean> {
        return this.connection_subject;
    }

    public MessageArrives(): Observable<[string, Uint8Array]> {
        return this.message_subject;
    }

    async Connect() {
        this.connection_subject.next(true);
    }

    override async Send(cmd: string, message?: string) {
        logger.info('Sending via MockSerialService');
        logger.info('cmd: ', cmd);
        logger.info('payload: ', message);
        if (cmd == 'get-config') {
            this.HandleGetConfig();
        } else {
            console.error('Unknown command: <' + cmd + '>');
        }

        return;
    }

    override async SendBinary(cmd: string, payload: Uint8Array) {
        logger.info('Sending Binary via MockSerialService');
        logger.info('cmd: ', cmd);
        logger.info('payload-size: ', payload.length);
    }

    private MockResponse(type: string, bin_payload: Uint8Array) {
        this.message_subject.next([type, bin_payload]);
    }

    private HandleGetConfig() {
        let configs: AnalogSwitchConfig[] = [];
        for (let i = 0; i < 16; i++) {
            let group_id;
            if (i >= 12 && i <= 15) {
                // D-pad
                group_id = 0;
            } else if (i == 2 || i == 4) {
                // for little fingers
                group_id = 1;
            } else {
                group_id = 4;
            }
            configs.push(new AnalogSwitchConfig({
                analog_switch_id: i,
                analog_switch_group_id: group_id
            }));
        };
        let groups: AnalogSwitchGroup[] = [];
        for (let i = 0; i < 8; i++) {
            let trigger_type = i < 4 ? TriggerType.RAPID_TRIGGER : TriggerType.STATIC_TRIGGER;

            let group: AnalogSwitchGroup = new AnalogSwitchGroup({
                analog_switch_group_id: i,
                trigger_type: trigger_type,
                rapid_trigger: new RapidTriggerConfig({
                    act: 0.6, rel: 0.4,
                    f_act: 3.0, f_rel: 0.2,
                }),
                static_trigger: new StaticTriggerConfig({
                    act: 1.2, rel: 0.8,
                })
            });
            groups.push(group);
        };

        // TODO: Choose human friendly button assignments(e.g. D-pad for left hand)
        let button_assignments: ButtonAssignment[] = [];
        // analog switches
        for (let i = 0; i < 16; i++) {
            button_assignments.push(new ButtonAssignment({
                button_id: new ButtonId({
                    type: ButtonType.PUSH,
                    push_button: new PushButtonSelector({
                        push_button_id: i
                    })
                }),
                switch_id: new SwitchId({
                    type: SwitchType.ANALOG_SWITCH,
                    id: i,
                })
            }));
        }
        // digital switches
        for (let i = 0; i < 2; i++) {
            button_assignments.push(new ButtonAssignment({
                button_id: new ButtonId({
                    type: ButtonType.PUSH,
                    push_button: new PushButtonSelector({
                        push_button_id: 30 + i
                    })
                }),
                switch_id: new SwitchId({
                    type: SwitchType.DIGITAL_SWITCH,
                    id: i,
                })
            }));
        }

        let udong_config: UdongConfig = new UdongConfig({
            analog_switch_configs: configs,
            analog_switch_groups: groups,
            button_assignments: button_assignments
        });

        // Mock response
        console.log('mocked response', udong_config);
        this.MockResponse('get-config', udong_config.serializeBinary());
    }
}
