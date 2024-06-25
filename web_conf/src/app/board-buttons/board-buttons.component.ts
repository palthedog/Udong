import { CommonModule } from '@angular/common';
import { Component, Input, Output, inject } from '@angular/core';
import { BehaviorSubject } from 'rxjs';
import { SwitchIdToButtonId, SwitchIdToGroupId, UdongConfig } from '../config';
import { AppConsts } from '../consts';
import { Logger } from '../logger';

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
  active_switch_id: number = 0;

  @Output()
  activeSwitchIdChanged = new BehaviorSubject<number>(0);

  app_consts = inject(AppConsts);

  // It is sorted based on the Switch ID (hardware order)
  switch_positions: [number, number][] = [
    [225, 57.5],  // Button 4
    [255, 62.5], // R1
    [285, 67.5], // L1
    [225, 90],  // Button 2

    [285, 100],  // L2
    [195, 95],  // Button 1
    [255, 95],   // R2
    [195, 62.5], // Button 3

    [35, 95],  // L3
    [180, 120],   // B15
    [165, 72.5],  // B14
    [90, 30],   // R3

    [150, 130],  // U
    [60, 70],    // L
    [120, 70],   // R
    [90, 60],    // D
  ];

  switch_ids: number[] = new Array(this.switch_positions.length).fill(0).map((_v, i) => i);

  getButtonColor(switch_id: number): string {
    let group_id = SwitchIdToGroupId(this.config, switch_id);
    return this.app_consts.configGroupColor(group_id);
  }

  getButtonName(switch_id: number): string {
    let button_id = SwitchIdToButtonId(this.config, switch_id);
    return this.app_consts.buttonName(button_id);
  }

  onClickButton(switch_id: number) {
    this.log.debug('button clicked', switch_id);
    this.activeSwitchIdChanged.next(switch_id);
  }
}
