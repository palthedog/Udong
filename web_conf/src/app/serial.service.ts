/// <reference types="dom-serial" />

import { Injectable } from '@angular/core';

import { BehaviorSubject, Observable, Subject, defer, filter } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class SerialService {

  private port?: SerialPort;

  private message_subject = new Subject<[string, string]>();
  private connection_subject = new BehaviorSubject<boolean>(false);

  public constructor() {
  }

  public ConnectionChanges(): Observable<boolean> {
    return this.connection_subject;
  }

  public MessageArrives(): Observable<[string, string]> {
    return this.message_subject;
  }

  public MessageReceiveFor(response_type: string): Observable<[string, string]> {
    return this.message_subject.asObservable().pipe(filter((pair) => {
      return pair[0] == response_type;
    }));
  }

  public async Connect() {
    const usbVendorId = 0x16c0;
    const usbProductId = 0x27dc;

    this.port = await navigator.serial.requestPort({
      filters: [
        { usbVendorId, usbProductId }
      ]
    });
    await this.port.open({ baudRate: 921600 })
    this.connection_subject.next(true);
    this.loopAsync(this.port);
  }

  public async Send(message: string) {
    if (!this.port) {
      console.error("Serial port is not connected");
      return;
    }
    const encoder = new TextEncoder();
    const writer = this.port.writable.getWriter();
    console.log('sending:', message);
    await writer.write(encoder.encode(message + "\n"));
    writer.releaseLock();
  }

  async loopAsync(port: SerialPort) {
    if (this.port == undefined) {
      return;
    }
    while (this.port.readable) {
      let line_reader = this.port.readable
        .pipeThrough(new TextDecoderStream())
        .pipeThrough(new TransformStream(new LineBreakTransformer()))
        .getReader();
      try {
        while (true) {
          const { value, done } = await line_reader.read();
          if (done) {
            console.log("Canceled");
            break;
          }
          const line = value;
          if (line.charAt(0) == '>') {
            // Ignore Teleplot messages
            continue;
          }
          this.message_subject.next(parseResponse(line));
        }
      } catch (error) {
        console.error("Error: Read" + error + "\n");
      } finally {
        console.log('release lock');
        line_reader.releaseLock();
        this.connection_subject.next(false);
      }
    }

    function parseResponse(response: string): [string, string] {
      let index = response.indexOf(':');
      let cmd = response;
      let arg = '';
      if (index >= 0) {
        cmd = response.slice(0, index);
        arg = response.slice(index + 1);
      }
      return [cmd, arg];
    }
  }
}

class LineBreakTransformer {

  buffer_: string = '';

  constructor() {
  }

  transform(chunk: string, controller: TransformStreamDefaultController) {
    this.buffer_ += chunk;
    const lines: Array<string> = this.buffer_.split('\n');
    // last one doesn't contain the line-break (could be an empty string)
    this.buffer_ = lines.pop()!;
    lines.forEach(line => {
      controller.enqueue(line);
    });
  }

  flush(controller: TransformStreamDefaultController) {
    controller.enqueue(this.buffer_);
  }
}
