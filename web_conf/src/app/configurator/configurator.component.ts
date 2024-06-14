import { Component, QueryList, ViewChild, ViewChildren, inject } from '@angular/core';
import { CommonModule } from '@angular/common';
import { MatCardModule } from '@angular/material/card';
import { SerialService } from '../serial.service';
import { AnalogSwitchConfigComponent } from '../analog-switch-config/analog-switch-config.component';
import { MatButtonModule } from '@angular/material/button'
import { GroupSelectorComponent } from '../group-selector/group-selector.component';
import { MatFormFieldModule } from '@angular/material/form-field';
import { FormsModule } from '@angular/forms';
import { MatRippleModule } from '@angular/material/core';
import { UdongConfig } from '../config';

@Component({
  selector: 'app-configurator',
  standalone: true,
  imports: [MatRippleModule, MatFormFieldModule, MatCardModule, CommonModule, AnalogSwitchConfigComponent, MatButtonModule, GroupSelectorComponent, FormsModule],
  templateUrl: './configurator.component.html',
  styleUrl: './configurator.component.scss'
})
export class ConfiguratorComponent {

  serial_service = inject(SerialService);

  config?: UdongConfig = undefined;

  @ViewChildren(AnalogSwitchConfigComponent)
  analog_switches!: QueryList<AnalogSwitchConfigComponent>;

  group_ids: Array<number> = [];

  constructor() {
    console.log('configurator init');
    this.serial_service.ConnectionChanges().subscribe((connected) => {
      console.log('connected', connected);
      if (connected) {
        this.serial_service.Send('get-config');
      }
    });

    this.serial_service.MessageReceiveFor('get-config').subscribe((v) => {
      console.log('config', v[0], v[1]);
      this.config = JSON.parse(v[1]);

      console.log('config');
      console.log(this.config);
      if (this.config) {
        this.group_ids = this.config.analog_switch_groups.map((group) => {
          return group.analog_switch_group_id;
        });
      } else {
        this.group_ids = [];
      }
    });
  }

  @ViewChild(AnalogSwitchConfigComponent)
  analog_switch_config_?: AnalogSwitchConfigComponent;

  setActiveGroupId(group_id: number) {
    console.log('active group changed', group_id);
    this.active_group_id = group_id;

    this.analog_switch_config_?.launchRipple();
  }

  active_group_id: number = 0;

  Save() {
    let str_config = JSON.stringify(this.config);
    console.log('save:', this.config);
    console.log('str:', str_config);
    this.serial_service.Send('save-config:' + str_config);
  }

}
