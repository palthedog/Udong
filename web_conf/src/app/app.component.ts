import { inject, Component } from '@angular/core';
import { CommonModule } from '@angular/common';
import { RouterOutlet } from '@angular/router';

import { MatSlideToggleModule } from '@angular/material/slide-toggle'

import { SerialService } from './serial.service';

import { AnalogSwitchConfigComponent } from "./analog-switch-config/analog-switch-config.component";
import { ConfiguratorComponent } from "./configurator/configurator.component"
import { SerialConnectorComponent } from './serial-connector/serial-connector.component'
import { TextCommanderComponent } from './text-commander/text-commander.component';

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
  title: string = '';

  serial_service = inject(SerialService);
  connected = this.serial_service.ConnectionChanges();

  constructor() {
    this.serial_service.ConnectionChanges().subscribe(connected => {
      this.title = connected ? 'Udong is Connected' : 'Udong is Not Connected';
    });
  }

  ngOnInit() {
    this.serial_service.MessageArrives().subscribe((line) => {
      console.log('got in subscribe: ' + line);
    });
  }

}
