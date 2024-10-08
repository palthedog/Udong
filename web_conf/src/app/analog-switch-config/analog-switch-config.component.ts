import { Component, EventEmitter, Input, Output, ViewChild, inject, signal } from '@angular/core';
import { CommonModule } from '@angular/common';
import { MatCardModule } from '@angular/material/card';
import { MatSliderModule } from '@angular/material/slider';
import { DepthSliderComponent } from '../depth-slider/depth-slider.component';
import { MatRipple, MatRippleModule } from '@angular/material/core';
import { MatSlideToggleModule } from '@angular/material/slide-toggle';
import { MatExpansionModule } from '@angular/material/expansion';
import { AppConsts } from '../consts';
import { GroupIconComponent } from '../group-icon/group-icon.component';
import { AnalogSwitchGroup, TriggerType } from '../../proto/config';
import { MatDivider } from '@angular/material/divider';

@Component({
  selector: 'app-analog-switch-config',
  standalone: true,
  imports: [MatDivider, MatExpansionModule, GroupIconComponent, MatSlideToggleModule, MatRippleModule, MatCardModule, MatSliderModule, DepthSliderComponent, CommonModule],
  templateUrl: './analog-switch-config.component.html',
  styleUrl: './analog-switch-config.component.scss'
})
export class AnalogSwitchConfigComponent {
  app_consts = inject(AppConsts);

  // Make its template accesible to TriggerType enum
  public TT = TriggerType;

  readonly detailedConfigOpenState = signal(false);

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
      this._config.trigger_type = TriggerType.RAPID_TRIGGER;
    } else {
      this._config.trigger_type = TriggerType.STATIC_TRIGGER;
    }
    this.configChange.next(this._config);
  }

  @ViewChild(MatRipple)
  ripple?: MatRipple = undefined;

  launchRipple() {
    this.ripple?.launch({ centered: true });
  }
}
