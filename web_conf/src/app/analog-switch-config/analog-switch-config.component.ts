import { Component, Input, Output } from '@angular/core';
import { CommonModule } from '@angular/common'
import { MatCardModule } from '@angular/material/card'
import { MatSliderModule } from '@angular/material/slider'
import { DepthSliderComponent } from '../depth-slider/depth-slider.component';
import { AnalogSwitchConfig, RapidTrigger, StaticTrigger } from '../config';

@Component({
  selector: 'app-analog-switch-config',
  standalone: true,
  imports: [MatCardModule, MatSliderModule, DepthSliderComponent, CommonModule],
  templateUrl: './analog-switch-config.component.html',
  styleUrl: './analog-switch-config.component.scss'
})
export class AnalogSwitchConfigComponent {
  rapid_trigger?: RapidTrigger;
  static_trigger?: StaticTrigger;

  private _config!: AnalogSwitchConfig;
  public get config(): AnalogSwitchConfig {
    return this._config;
  }
  @Input()
  public set config(value: AnalogSwitchConfig) {
    this._config = value;

    this.rapid_trigger = undefined;
    this.static_trigger = undefined;

    console.log('config value: ', value);
    if (value.trigger.type == 'rapid-trigger') {
      this.rapid_trigger = value.trigger as RapidTrigger;
    } else if (value.trigger.type == 'static-trigger') {
      this.static_trigger = value.trigger as StaticTrigger;
    }
  }

  ngOnInit() {

  }
}
