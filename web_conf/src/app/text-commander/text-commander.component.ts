import { Component, inject } from '@angular/core';
import { SerialService } from '../serial.service';

@Component({
  selector: 'app-text-commander',
  standalone: true,
  template: `
    <div>
        <input #command type="text" id="cmd-input" />
        <input type="button" value="Send" (click)="sendCommand(command.value);" />
    </div>
  `,
  styleUrl: './text-commander.component.scss'
})
export class TextCommanderComponent {

  serial_service = inject(SerialService);
  hidden = false;

  ngOnInit() {
    this.serial_service.ConnectionChanges().subscribe(connected => {
      this.hidden = !connected;
    });
  }

  sendCommand(command: string) {
    console.log('command:', command);
    this.serial_service.Send(command);
  }
}
