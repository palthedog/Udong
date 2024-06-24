import { Component, EventEmitter, Input, Output, ViewChild, inject } from '@angular/core';
import { CommonModule } from '@angular/common'
import { MatCardModule } from '@angular/material/card'
import { MatSliderModule } from '@angular/material/slider'
import { DepthSliderComponent } from '../depth-slider/depth-slider.component';
import { MatRipple, MatRippleModule } from '@angular/material/core';
import { AnalogSwitchGroup } from '../config';
import { MatSlideToggleModule } from '@angular/material/slide-toggle'
import { AppConsts } from '../consts';
import { GroupIconComponent } from '../group-icon/group-icon.component';

@Component({
  selector: 'app-analog-switch-config',
  standalone: true,
  imports: [GroupIconComponent, MatSlideToggleModule, MatRippleModule, MatCardModule, MatSliderModule, DepthSliderComponent, CommonModule],
  templateUrl: './analog-switch-config.component.html',
  styleUrl: './analog-switch-config.component.scss'
})
export class AnalogSwitchConfigComponent {
  app_consts = inject(AppConsts);

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

  enableRapidTrigger(enable: boolean) {
    if (enable) {
      this._config.trigger_type = 'rapid-trigger';
    } else {
      this._config.trigger_type = 'static-trigger';
    }
    this.configChange.next(this._config);
  }

  @ViewChild(MatRipple)
  ripple?: MatRipple = undefined;

  launchRipple() {
    this.ripple?.launch({ centered: true });
  }
}
