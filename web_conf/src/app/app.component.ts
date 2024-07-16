import { inject, Component, Output } from '@angular/core';
import { CommonModule } from '@angular/common';
import { RouterOutlet } from '@angular/router';

import { MatSlideToggleModule } from '@angular/material/slide-toggle';

import { SerialServiceInterface } from './serial/serial.service';

import { AnalogSwitchConfigComponent } from "./analog-switch-config/analog-switch-config.component";
import { ConfiguratorComponent } from "./configurator/configurator.component";
import { SerialConnectorComponent } from './serial-connector/serial-connector.component';
import { TextCommanderComponent } from './text-commander/text-commander.component';
import { logger } from './logger';
import { Observable, Subscription } from 'rxjs';

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
  serial_service = inject(SerialServiceInterface);

  title: string = '';
  connected?: Observable<boolean>;

  subscriptions: Subscription = new Subscription();

  constructor() {
    logger.debug('AppComponent constructor', this.serial_service);
    this.connected = this.serial_service.ConnectionChanges();
  }

  ngOnInit() {
    logger.debug('AppComponent initialized', this.serial_service);
    this.subscriptions.add(this.serial_service.ConnectionChanges().subscribe(connected => {
      this.title = connected ? 'Udong is Connected' : 'Udong is Not Connected';
    }));
  }

  ngOnDestroy() {
    this.subscriptions.unsubscribe();
  }
}
