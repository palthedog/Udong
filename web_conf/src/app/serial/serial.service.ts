/// <reference types="dom-serial" />

import { Injectable } from '@angular/core';

import { BehaviorSubject, Observable, Subject, defer, filter } from 'rxjs';

export abstract class SerialServiceInterface {
  abstract ConnectionChanges(): Observable<boolean>;

  abstract MessageArrives(): Observable<[string, Uint8Array]>;

  abstract Connect(): Promise<void>;

  abstract Send(cmd: string, message?: string): Promise<void>;
  abstract SendBinary(cmd: string, payload: Uint8Array): Promise<void>;

  MessageReceiveFor(response_type: string): Observable<[string, Uint8Array]> {
    return this.MessageArrives().pipe(filter((pair) => {
      return pair[0] == response_type;
    }));
  }
}

@Injectable()
export class SerialService extends SerialServiceInterface {

  private port?: SerialPort;

  private message_subject = new Subject<[string, Uint8Array]>();
  private connection_subject = new BehaviorSubject<boolean>(false);

  private parser = new MessageParser();
  private text_encoder = new TextEncoder();

  public constructor() {
    super();
  }

  public ConnectionChanges(): Observable<boolean> {
    return this.connection_subject;
  }

  public MessageArrives(): Observable<[string, Uint8Array]> {
    return this.message_subject;
  }

  public async Connect() {
    const usbVendorId = 0x16c0;
    const usbProductId = 0x27dc;

    this.port = await navigator.serial.requestPort({
      filters: [
        { usbVendorId, usbProductId }
      ]
    });
    await this.port.open({ baudRate: 921600 });
    this.connection_subject.next(true);
    this.loopAsync(this.port);
  }

  override async Send(cmd: string, message?: string) {
    if (!this.port) {
      console.error("Serial port is not connected");
      return;
    }

    console.log('Sending: cmd: ', cmd, 'payload: ', message);
    const encoder = new TextEncoder();
    const writer = this.port.writable.getWriter();
    if (message == undefined) {
      await writer.write(encoder.encode(cmd + '\n'));
    } else {
      await writer.write(encoder.encode(cmd + ':' + message + '\n'));
    }
    writer.releaseLock();
  }

  override async SendBinary(cmd: string, payload: Uint8Array) {
    if (!this.port) {
      console.error("Serial port is not connected");
      return;
    }
    const writer = this.port.writable.getWriter();
    console.log('sending: <binary> size: ', payload.length);
    await writer.write(this.text_encoder.encode(cmd + '@' + payload.length + '#'));
    await writer.write(payload);
    console.log('wrote');
    writer.releaseLock();
  }

  async loopAsync(port: SerialPort) {
    if (this.port == undefined) {
      return;
    }
    while (this.port.readable) {
      let reader = this.port.readable.getReader();
      try {
        while (true) {
          const result = await reader.read();
          if (result.done) {
            console.log("Canceled");
            break;
          }

          if (!(result.value instanceof Uint8Array)) {
            console.error("Error: Read value is not Uint8Array");
            break;
          }

          result.value.forEach((byte: number) => {
            let ret = this.parser.push(byte);
            if (ret.done) {
              this.message_subject.next([ret.cmd!, ret.payload!]);
            }
          });
        }
      } catch (error) {
        console.error("Error: Read" + error + "\n");
      } finally {
        console.log('release lock');
        reader.releaseLock();
        this.connection_subject.next(false);
      }
    }
  }
}

enum ParserState {
  ReadingCommand,
  ReadingSize,
  ReadingBinaryPayload
};

interface ParseResult {
  done: boolean;
  cmd?: string;
  payload?: Uint8Array;
};

class MessageParser {
  state_: ParserState = ParserState.ReadingCommand;

  cmd_: string = '';
  payload_size_: number = 0;

  payload_: Uint8Array = new Uint8Array(0);
  payload_offset_: number = 0;

  constructor() {
  }

  push(byte: number): ParseResult {
    switch (this.state_) {
      case ParserState.ReadingCommand: {
        let ch = String.fromCharCode(byte);
        if (ch == '@') {
          this.state_ = ParserState.ReadingSize;
          console.log('received @: cmd: ', this.cmd_);
        } else {
          this.cmd_ += ch;
        }
        break;
      }
      case ParserState.ReadingSize: {
        let ch = String.fromCharCode(byte);
        if (ch == '#') {
          console.log('received #: payload size: ', this.payload_size_);
          this.state_ = ParserState.ReadingBinaryPayload;
          this.payload_ = new Uint8Array(this.payload_size_);
          this.payload_offset_ = 0;
        } else {
          if (ch >= '0' && ch <= '9') {
            this.payload_size_ = this.payload_size_ * 10 + parseInt(ch);
          } else {
            console.error('Error: ReadingSize: Invalid character: ', ch);
            this.reset();
            return { done: false };
          }
        }
        break;
      }
      case ParserState.ReadingBinaryPayload: {
        this.payload_[this.payload_offset_] = byte;
        this.payload_offset_ += 1;
        if (this.payload_offset_ == this.payload_size_) {
          let ret = { done: true, cmd: this.cmd_, payload: this.payload_ };
          this.reset();
          return ret;
        }
        break;
      }
    }
    return { done: false };
  }

  reset() {
    this.cmd_ = '';
    this.payload_size_ = 0;
    this.payload_ = new Uint8Array(0);
  }
}
