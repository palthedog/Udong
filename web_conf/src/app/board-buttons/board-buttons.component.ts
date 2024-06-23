import { Component, Input } from '@angular/core';
import { SwitchIdToGroupId, UdongConfig } from '../config';
import { CommonModule } from '@angular/common';

@Component({
  selector: 'app-board-buttons',
  standalone: true,
  imports: [CommonModule],
  templateUrl: './board-buttons.component.html',
  styleUrl: './board-buttons.component.scss'
})
export class BoardButtonsComponent {
  @Input()
  config!: UdongConfig;

  // It is sorted based on the Switch ID (hardware order)
  switch_positions: [number, number][] = [
    [150, 130],  // U
    [120, 70],   // R
    [90, 60],    // D
    [60, 70],    // L

    [195, 95],  // Button 1
    [225, 90],  // Button 2
    [195, 62.5], // Button 3
    [225, 57.5],  // Button 4

    [285, 67.5], // L1
    [255, 62.5], // R2

    [285, 100],  // L2
    [255, 95],   // R2

    [35, 95],  // L3
    [90, 30],   // R3

    [165, 72.5],  // B14
    [180, 120],   // B15
  ];

  switch_ids: number[] = new Array(this.switch_positions.length).fill(0).map((_v, i) => i);

  getButtonColor(switch_id: number): string {
    let group_id = SwitchIdToGroupId(this.config, switch_id);
    let hue = group_id * (360.0 / 16);
    return 'hsl(' + hue + ' 80% 60%)';
  }

  onClickButton(switch_id: number) {
    console.log('button clicked', switch_id);
  }
}
