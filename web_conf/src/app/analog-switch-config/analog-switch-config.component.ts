import { Component, EventEmitter, Input, Output, ViewChild } from '@angular/core';
import { CommonModule } from '@angular/common'
import { MatCardModule } from '@angular/material/card'
import { MatSliderModule } from '@angular/material/slider'
import { DepthSliderComponent } from '../depth-slider/depth-slider.component';
import { MatRipple, MatRippleModule } from '@angular/material/core';
import { AnalogSwitchGroup } from '../config';

@Component({
  selector: 'app-analog-switch-config',
  standalone: true,
  imports: [MatRippleModule, MatCardModule, MatSliderModule, DepthSliderComponent, CommonModule],
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

  @ViewChild(MatRipple)
  ripple?: MatRipple = undefined;

  launchRipple() {
    this.ripple?.launch({ centered: true });
  }

  actChange(event: any) {
    console.log('actChange', event);
  }

  ngOnInit() {

  }
}
