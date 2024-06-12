import { inject, Input, Component, Output, EventEmitter } from '@angular/core';

import { SerialService } from '../serial.service';

import { MatButtonModule } from '@angular/material/button'

@Component({
  selector: 'app-serial-connector',
  standalone: true,
  imports: [MatButtonModule],
  templateUrl: './serial-connector.component.html',
  styleUrl: './serial-connector.component.scss'
})
export class SerialConnectorComponent {

  @Input()
  button_name!: string;

  serial_service = inject(SerialService);

  constructor() {

  }

  async Connect() {
    this.serial_service.Connect();
  }

}
