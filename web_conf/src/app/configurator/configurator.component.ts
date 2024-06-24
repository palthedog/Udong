import { Component, QueryList, ViewChild, ViewChildren, inject } from '@angular/core';
import { CommonModule } from '@angular/common';
import { MatCardModule } from '@angular/material/card';
import { SerialServiceInterface } from '../serial.service';
import { AnalogSwitchConfigComponent } from '../analog-switch-config/analog-switch-config.component';
import { MatButtonModule } from '@angular/material/button'
import { GroupSelectorComponent } from '../group-selector/group-selector.component';
import { MatFormFieldModule } from '@angular/material/form-field';
import { FormsModule } from '@angular/forms';
import { MatRipple, MatRippleModule } from '@angular/material/core';
import { AnalogSwitchAssignment, AnalogSwitchGroup, ButtonAssignment, SwitchIdToGroupId, UdongConfig } from '../config';
import { BoardButtonsComponent } from '../board-buttons/board-buttons.component';
import { MatSelectModule } from '@angular/material/select';
import { MatDividerModule } from '@angular/material/divider';
import { AppConsts } from '../consts';

@Component({
  selector: 'app-configurator',
  standalone: true,
  imports: [MatDividerModule, MatSelectModule, BoardButtonsComponent, MatRippleModule, MatFormFieldModule, MatCardModule, CommonModule, AnalogSwitchConfigComponent, MatButtonModule, GroupSelectorComponent, FormsModule],
  templateUrl: './configurator.component.html',
  styleUrl: './configurator.component.scss'
})
export class ConfiguratorComponent {
  serial_service = inject(SerialServiceInterface);
  consts = inject(AppConsts);

  active_switch_id: number = 0;
  active_group_id: number = 0;

  config?: UdongConfig = undefined;

  @ViewChildren(AnalogSwitchConfigComponent)
  analog_switches!: QueryList<AnalogSwitchConfigComponent>;

  @ViewChild(AnalogSwitchConfigComponent)
  analog_switch_config_?: AnalogSwitchConfigComponent;

  @ViewChild(MatRipple)
  ripple!: MatRipple;

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

  setActiveSwitchId(switch_id: number) {
    console.log('active switch changed', switch_id);
    this.active_switch_id = switch_id;
    this.setActiveGroupId(SwitchIdToGroupId(this.config!, this.active_switch_id));
    this.ripple.launch({ centered: true });
  }

  onGroupSelected() {
    console.log('onGroupSelected');
    this.setActiveGroupId(SwitchIdToGroupId(this.config!, this.active_switch_id));
  }

  setActiveGroupId(group_id: number) {
    console.log('active group changed', group_id);
    this.active_group_id = group_id;
  }

  activeSwitchAssignment(): AnalogSwitchAssignment {
    return this.config!.analog_switch_assignments.find((assignment) => {
      return assignment.analog_switch_id == this.active_switch_id;
    })!;
  }

  activeSwitchGroup(): AnalogSwitchGroup {
    return this.config!.analog_switch_groups.find((group) => {
      return group.analog_switch_group_id == this.active_group_id;
    })!;
  }

  activeButtonAssignment(): ButtonAssignment {
    return this.config!.button_assignments.find((button_assignment) => {
      return button_assignment.switch_id == this.active_switch_id;
    })!;
  }

  changeButtonMapping(button_id: number) {
    console.log('button mapping:', button_id);
  }

  Save() {
    let str_config = JSON.stringify(this.config);
    console.log('save:', this.config);
    console.log('str:', str_config);
    this.serial_service.Send('save-config:' + str_config);
  }
}
