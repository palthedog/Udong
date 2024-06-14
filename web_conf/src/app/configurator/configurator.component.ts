import { Component, QueryList, ViewChildren, inject } from '@angular/core';
import { CommonModule } from '@angular/common';
import { MatCardModule } from '@angular/material/card';
import { SerialService } from '../serial.service';
import { interval } from 'rxjs';
import { AnalogSwitchAssignment, UdongConfig } from '../config';
import { AnalogSwitchConfigComponent } from '../analog-switch-config/analog-switch-config.component';
import { MatButtonModule } from '@angular/material/button'
import { GroupSelectorComponent } from '../group-selector/group-selector.component';
import { MatFormFieldModule } from '@angular/material/form-field';
import { FormsModule } from '@angular/forms';

@Component({
  selector: 'app-configurator',
  standalone: true,
  imports: [MatFormFieldModule, MatCardModule, CommonModule, AnalogSwitchConfigComponent, MatButtonModule, GroupSelectorComponent, FormsModule],
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

  Save() {
    console.log('save:', this.config);
  }

}


