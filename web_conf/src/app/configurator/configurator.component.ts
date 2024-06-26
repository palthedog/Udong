import { CommonModule } from '@angular/common';
import { Component, QueryList, ViewChild, ViewChildren, inject } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { MatButtonModule } from '@angular/material/button';
import { MatCardModule } from '@angular/material/card';
import { MatRipple, MatRippleModule } from '@angular/material/core';
import { MatDividerModule } from '@angular/material/divider';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatSelectModule } from '@angular/material/select';
import { AnalogSwitchConfigComponent } from '../analog-switch-config/analog-switch-config.component';
import { BoardButtonsComponent } from '../board-buttons/board-buttons.component';
import { AnalogSwitchAssignment, AnalogSwitchGroup, ButtonAssignment, ButtonId, UdongConfig } from '../../proto/config';
import { AppConsts } from '../consts';
import { GroupSelectorComponent } from '../group-selector/group-selector.component';
import { SerialServiceInterface } from '../serial/serial.service';
import { Logger } from '../logger';
import { SwitchIdToGroupId, compareButtonIds } from '../config_util';

@Component({
  selector: 'app-configurator',
  standalone: true,
  imports: [MatDividerModule, MatSelectModule, BoardButtonsComponent, MatRippleModule, MatFormFieldModule, MatCardModule, CommonModule, AnalogSwitchConfigComponent, MatButtonModule, GroupSelectorComponent, FormsModule],
  templateUrl: './configurator.component.html',
  styleUrl: './configurator.component.scss'
})
export class ConfiguratorComponent {
  log = inject(Logger);
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
  ripple?: MatRipple;

  group_ids: Array<number> = [];

  constructor() {
    this.serial_service.ConnectionChanges().subscribe((connected) => {
      this.log.info('connected', connected);
      if (connected) {
        this.serial_service.Send('get-config');
      }
    });

    this.serial_service.MessageReceiveFor('get-config').subscribe((v) => {
      this.config = UdongConfig.deserializeBinary(v[1]);
      this.log.info('Config received');
      this.log.info(this.config.toObject());
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
    this.log.info('active switch changed', switch_id);
    this.active_switch_id = switch_id;
    this.setActiveGroupId(SwitchIdToGroupId(this.config!, this.active_switch_id));
    if (this.ripple) {
      this.ripple.launch({ centered: true });
    }
  }

  onGroupSelected() {
    this.log.info('onGroupSelected');
    this.setActiveGroupId(SwitchIdToGroupId(this.config!, this.active_switch_id));
  }

  setActiveGroupId(group_id: number) {
    this.log.info('active group changed', group_id);
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

  compareValue(a: ButtonId, b: ButtonId): boolean {
    return compareButtonIds(a, b);
  }

  Save() {
    if (this.config == undefined) {
      return;
    }
    this.log.info('save:', this.config);
    this.serial_service.SendBinary('save-config', this.config.serializeBinary());
  }
}
