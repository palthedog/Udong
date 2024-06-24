import { Injectable } from '@angular/core';

import { BehaviorSubject, Observable, Subject, defer, filter } from 'rxjs';
import { AnalogSwitchAssignment, AnalogSwitchGroup, ButtonAssignment, UdongConfig } from '../config';
import { SerialServiceInterface } from './serial.service';

@Injectable()
export class MockSerialService extends SerialServiceInterface {
    private message_subject = new Subject<[string, string]>();
    private connection_subject = new BehaviorSubject<boolean>(false);

    public ConnectionChanges(): Observable<boolean> {
        return this.connection_subject;
    }

    public MessageArrives(): Observable<[string, string]> {
        return this.message_subject;
    }

    async Connect() {
        this.connection_subject.next(true);
    }

    async Send(message: string) {
        console.log('send via MockSerialService', message);
        if (message == 'get-config') {
            this.HandleGetConfig();
        }

        return;
    }

    private MockResponse(type: string, obj: Object) {
        this.message_subject.next([type, JSON.stringify(obj)]);
    }

    private HandleGetConfig() {
        let assignments: AnalogSwitchAssignment[] = [];
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
            assignments.push(
                {
                    analog_switch_id: i,
                    analog_switch_group_id: group_id
                });
        };
        let groups: AnalogSwitchGroup[] = [];
        for (let i = 0; i < 8; i++) {
            let trigger_type = i < 4 ? 'rapid-trigger' : 'static-trigger';

            let group: AnalogSwitchGroup = {
                analog_switch_group_id: i,
                trigger_type: trigger_type,
                rapid_trigger: {
                    act: 0.6, rel: 0.4,
                    f_act: 3.0, f_rel: 0.2,
                },
                static_trigger: {
                    act: 1.2, rel: 0.8,
                }
            };
            groups.push(group);
        };

        let button_assignments: ButtonAssignment[] = [];
        for (let i = 0; i < 16; i++) {
            button_assignments.push({
                button_id: {
                    type: 'push',
                    push_button: {
                        push_button_id: i
                    }
                },
                switch_id: i,
            });
        }

        let udong_config: UdongConfig = {
            analog_switch_assignments: assignments,
            analog_switch_groups: groups,
            button_assignments: button_assignments
        };

        // Mock response
        this.MockResponse('get-config', udong_config);
    }
}
