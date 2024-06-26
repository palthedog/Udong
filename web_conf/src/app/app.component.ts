import { inject, Component } from '@angular/core';
import { CommonModule } from '@angular/common';
import { RouterOutlet } from '@angular/router';

import { MatSlideToggleModule } from '@angular/material/slide-toggle';

import { SerialServiceInterface } from './serial/serial.service';

import { AnalogSwitchConfigComponent } from "./analog-switch-config/analog-switch-config.component";
import { ConfiguratorComponent } from "./configurator/configurator.component";
import { SerialConnectorComponent } from './serial-connector/serial-connector.component';
import { TextCommanderComponent } from './text-commander/text-commander.component';
import { Logger } from './logger';

@Component({
  selector: 'app-root',
  standalone: true,
  templateUrl: './app.component.html',
  styleUrl: './app.component.scss',
  imports: [
    ConfiguratorComponent, MatSlideToggleModule, RouterOutlet, SerialConnectorComponent, TextCommanderComponent, CommonModule, AnalogSwitchConfigComponent
  ]
})
export class AppComponent {
  log = inject(Logger);

  title: string = '';

  serial_service = inject(SerialServiceInterface);
  connected = this.serial_service.ConnectionChanges();

  constructor() {
    this.serial_service.ConnectionChanges().subscribe(connected => {
      this.title = connected ? 'Udong is Connected' : 'Udong is Not Connected';
    });
  }

  ngOnInit() {
  }

}
