import { Injectable } from '@angular/core';
import { SerialServiceInterface } from './serial.service';

import { BehaviorSubject, Observable, Subject, defer, filter } from 'rxjs';
import { AnalogSwitchAssignment, AnalogSwitchGroup, UdongConfig } from './config';

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
            let group: AnalogSwitchGroup;
            if (i < 4) {
                group = {
                    analog_switch_group_id: i,
                    trigger_type: 'rapid-trigger',
                    rapid_trigger: {
                        act: 0.6, rel: 0.4,
                        f_act: 3.0, f_rel: 0.2,
                    }
                }
            } else {
                group = {
                    analog_switch_group_id: i,
                    trigger_type: 'static-trigger',
                    static_trigger: {
                        act: 1.2, rel: 0.8,
                    }
                }
            }
            groups.push(group);
        };

        let udong_config: UdongConfig = {
            analog_switch_assignments: assignments,
            analog_switch_groups: groups
        };

        // Mock response
        this.MockResponse('get-config', udong_config);
    }
}
