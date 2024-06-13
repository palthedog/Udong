import { Component, QueryList, ViewChildren, inject } from '@angular/core';
import { CommonModule } from '@angular/common';
import { MatCardModule } from '@angular/material/card';
import { SerialService } from '../serial.service';
import { interval } from 'rxjs';
import { AnalogSwitchConfig, UdongConfig } from '../config';
import { AnalogSwitchConfigComponent } from '../analog-switch-config/analog-switch-config.component';
import { MatButtonModule } from '@angular/material/button'

@Component({
  selector: 'app-configurator',
  standalone: true,
  imports: [MatCardModule, CommonModule, AnalogSwitchConfigComponent, MatButtonModule],
  templateUrl: './configurator.component.html',
  styleUrl: './configurator.component.scss'
})
export class ConfiguratorComponent {

  serial_service = inject(SerialService);

  config?: UdongConfig = undefined;

  @ViewChildren(AnalogSwitchConfigComponent)
  analog_switches!: QueryList<AnalogSwitchConfigComponent>;

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
    });
  }

  Save() {
    console.log('save:', this.config);
    console.log('save:', this.analog_switches.first.rapid_trigger?.act);
  }
}


