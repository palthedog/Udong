import { CommonModule } from '@angular/common';
import { Component, Input, Output, inject } from '@angular/core';
import { BehaviorSubject } from 'rxjs';
import { AppConsts } from '../consts';
import { Logger } from '../logger';
import { SwitchId, SwitchType, UdongConfig } from '../../proto/config';
import { SwitchIdToButtonId, SwitchIdToGroupId } from '../config_util';

interface SwitchConfig {
  switch_id: SwitchId,

  x: number;
  y: number;
}

@Component({
  selector: 'app-board-buttons',
  standalone: true,
  imports: [CommonModule],
  templateUrl: './board-buttons.component.html',
  styleUrl: './board-buttons.component.scss'
})
export class BoardButtonsComponent {
  log = inject(Logger);

  @Input()
  config!: UdongConfig;

  @Input()
  active_switch_id: SwitchId = new SwitchId({ type: SwitchType.ANALOG_SWITCH, id: 0 });

  @Output()
  activeSwitchIdChanged = new BehaviorSubject<SwitchId>(
    new SwitchId({ type: SwitchType.ANALOG_SWITCH, id: 0 }));

  app_consts = inject(AppConsts);

  // It is sorted based on the Switch ID (hardware order)

  analog_switch_positions: [number, number][] = [
    [225, 57.5],  // Button 4
    [255, 62.5], // R1
    [285, 67.5], // L1
    [195, 62.5], // Button 3

    [285, 100],  // L2
    [195, 95],  // Button 1
    [255, 95],   // R2
    [225, 90],  // Button 2

    [35, 95],  // L3
    [180, 120],   // B15
    [165, 72.5],  // B14
    [90, 30],   // R3

    [150, 130],  // U
    [60, 70],    // L
    [120, 70],   // R
    [90, 60],    // D
  ];

  digital_switch_positions: [number, number][] = [
    [27.25, 17.7],
    [49.65, 17.7],
  ];

  get switch_configs(): SwitchConfig[] {
    return this.analog_switch_positions.map((pos, i) => {
      return {
        switch_id: new SwitchId({ type: SwitchType.ANALOG_SWITCH, id: i }),
        x: pos[0],
        y: pos[1],
      };
    }).concat(this.digital_switch_positions.map((pos, i) => {
      return {
        switch_id: new SwitchId({ type: SwitchType.DIGITAL_SWITCH, id: i }),
        x: pos[0],
        y: pos[1],
      };
    }));
  };

  getButtonColor(switch_id: SwitchId): string {
    if (switch_id.type === SwitchType.DIGITAL_SWITCH) {
      return '#ccc';
    } else if (switch_id.type === SwitchType.ANALOG_SWITCH) {
      let group_id = SwitchIdToGroupId(this.config, switch_id);
      return this.app_consts.configGroupColor(group_id!);
    }
    this.log.error('Unknown switch_id.type: ', switch_id);

    return '#fff';
  }

  getButtonName(switch_id: SwitchId): string {
    let button_id = SwitchIdToButtonId(this.config, switch_id);
    return this.app_consts.buttonName(button_id);
  }

  onClickButton(switch_id: SwitchId) {
    this.log.debug('button clicked', switch_id);
    this.activeSwitchIdChanged.next(switch_id);
  }
}
