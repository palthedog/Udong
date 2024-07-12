import { Component, inject } from '@angular/core';
import { SerialServiceInterface } from '../serial/serial.service';
import { logger, Logger } from '../logger';
import { connect } from 'http2';
import { delay } from 'rxjs';

@Component({
  selector: 'app-button-preview',
  standalone: true,
  imports: [],
  templateUrl: './button-preview.component.html',
  styleUrl: './button-preview.component.scss'
})
export class ButtonPreviewComponent {

  serial_service = inject(SerialServiceInterface);

  text: string = 'ButtonPreviewComponent';

  press_mm: number = 0;

  connected: boolean = false;

  constructor() {
  }

  ngOnInit() {
    this.text = 'foo bar';
    logger.debug('ButtonPreviewComponent initialized');

    this.serial_service.ConnectionChanges().subscribe((value) => {
      this.connected = value;
    });;

    this.serial_service.MessageReceiveFor('get').subscribe(([cmd, v]) => {
      if (v != null && typeof v == 'object') {
        this.press_mm = v['analog_switches'][0]['press_mm'];
      }

      if (this.connected) {
        //setTimeout(() => {
        // It looks nice to me
        // https://nagix.github.io/chartjs-plugin-streaming/latest/ja/samples/charts/line-horizontal.html
        this.serial_service.Send('get');
        //}, 1);
      } else {
        logger.info('Serial port is disconnected');
      }
    });
    this.serial_service.Send('get');
  }

  ngOnShow() {
    logger.debug('ButtonPreviewComponent shown');
  }

}
