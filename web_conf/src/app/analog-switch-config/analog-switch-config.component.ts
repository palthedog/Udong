import { Component, EventEmitter, Input, Output } from '@angular/core';
import { CommonModule } from '@angular/common'
import { MatCardModule } from '@angular/material/card'
import { MatSliderModule } from '@angular/material/slider'
import { DepthSliderComponent } from '../depth-slider/depth-slider.component';
import { AnalogSwitchAssignment, AnalogSwitchGroup, RapidTrigger, StaticTrigger } from '../config';

@Component({
  selector: 'app-analog-switch-config',
  standalone: true,
  imports: [MatCardModule, MatSliderModule, DepthSliderComponent, CommonModule],
  templateUrl: './analog-switch-config.component.html',
  styleUrl: './analog-switch-config.component.scss'
})
export class AnalogSwitchConfigComponent {
  @Output()
  configChange = new EventEmitter<AnalogSwitchGroup>();

  private _config!: AnalogSwitchGroup;
  public get config(): AnalogSwitchGroup {
    return this._config;
  }
  @Input()
  public set config(value: AnalogSwitchGroup) {
    console.log('set cfg', value);
    this._config = value;
  }

  actChange(event: any) {
    console.log('actChange', event);
  }

  ngOnInit() {

  }
}
