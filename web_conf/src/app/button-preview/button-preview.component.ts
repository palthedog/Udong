import { Component, Directive, ElementRef, HostListener, inject, Injectable, Input, ViewChild } from '@angular/core';
import { SerialServiceInterface } from '../serial/serial.service';
import { logger } from '../logger';
import { BaseChartDirective } from 'ng2-charts';
import { ChartConfiguration, ChartData, ChartOptions, TooltipItem } from 'chart.js';
import { AnalogSwitchGroup } from '../../proto/config';
import { GetAnalogSwitchStateRequest, GetAnalogSwitchStateResponse } from '../../proto/rpc';
import { max, Subject, Subscription } from 'rxjs';


@Injectable({ providedIn: 'root' })
export class GetAnalogSwitchStateService {
  serial_service = inject(SerialServiceInterface);

  running: boolean = false;

  private analog_switch_state_subject: Subject<GetAnalogSwitchStateResponse> = new Subject();

  get analog_switch_state_received() {
    return this.analog_switch_state_subject.asObservable();
  }

  active_analog_switch_id: number = 0;

  constructor() {
    console.log('GetAnalogSwitchStateService constructed');
    this.serial_service.ConnectionChanges().subscribe((connected) => {
      if (connected) {
        this.Run();
      } else {
        this.Stop();
      }
    });

    this.serial_service.MessageReceiveFor('get-analog-switch-state').subscribe(([cmd, v]) => {
      if (!this.running) {
        return;
      }
      let res = GetAnalogSwitchStateResponse.deserializeBinary(v);
      this.analog_switch_state_subject.next(res);

      setTimeout(() => {
        this.SendGetAnalogSwitchState();
      }, 5);
    });
  }

  Run() {
    if (this.running) {
      return;
    }
    this.running = true;
    this.SendGetAnalogSwitchState();
  }

  Stop() {
    this.running = false;
  }

  SetActiveAnalogSwitchId(id: number) {
    this.active_analog_switch_id = id;
    this.Run();
  }

  private SendGetAnalogSwitchState() {
    let request = new GetAnalogSwitchStateRequest();
    request.analog_switch_id = this.active_analog_switch_id;
    this.serial_service.SendBinary('get-analog-switch-state', request.serializeBinary());
  }
}

const kIndexPressMm = 0;
const kIndexButtonState = 1;
const kIndexActuationPoint = 2;
const kIndexReleasePoint = 3;
const kIndexPollingRate = 4;

const kDataWindowMs = 3000;

const xAxisID = 'xAxis';
const yMmAxisID = 'yMmAxis';
const yOnOffAxisID = 'yOnOffAxis';
const yHzAxisID = 'yHzAxis';

@Component({
  selector: 'app-button-preview',
  standalone: true,
  imports: [BaseChartDirective],
  templateUrl: './button-preview.component.html',
  styleUrl: './button-preview.component.scss'
})
export class ButtonPreviewComponent {

  serial_service = inject(SerialServiceInterface);
  analog_switch_state_service = inject(GetAnalogSwitchStateService);

  subscriptions: Subscription = new Subscription();

  press_mm: number = 0;
  polling_rate: number = 0;

  connected: boolean = false;
  paused: boolean = false;

  private zoom_level: number = 1;

  @Input()
  set active_analog_switch_id(v: number) {
    this.ClearData();
    this.analog_switch_state_service.SetActiveAnalogSwitchId(v);
  }

  @Input()
  analog_switch_group_config: AnalogSwitchGroup | null = null;

  @ViewChild(BaseChartDirective)
  chart!: BaseChartDirective;

  @HostListener('wheel', ['$event'])
  onMousewheel(event: any) {
    let chart = this.chart.chart;
    if (!chart) {
      logger.warn('Wheel event detected but the chart is not available');
      return;
    }
    let x_min = chart.options?.scales?.[xAxisID]?.min;
    let x_max = chart.options?.scales?.[xAxisID]?.max;
    if (typeof x_min != 'number' || typeof x_max != 'number') {
      logger.warn('Wheel event detected but the chart has no data');
      return;
    }
    // Pause the chart update while zooming.
    this.SetPause(true);

    let new_zoom_level = this.zoom_level * (1 - event.deltaY / 1000);
    new_zoom_level = Math.min(200, Math.max(1.0, new_zoom_level));

    let chart_width = chart.chartArea.right - chart.chartArea.left;
    // x_percent is the position of the mouse cursor in the chart area.
    // 0 means the left edge of the chart area, 1 means the right edge.
    let x_percent = (event.offsetX - chart.chartArea.left) / chart_width;

    let width_us = (x_max - x_min) * this.zoom_level;
    let new_width_us = width_us / new_zoom_level;

    // x_us is the position of the mouse cursor in the time axis.
    let x_us = x_min + x_percent * (x_max - x_min);
    let max_us = x_us + new_width_us * (1 - x_percent);
    let min_us = x_us - new_width_us * x_percent;

    this.SetXAxisRange(min_us, max_us);

    this.zoom_level = new_zoom_level;
    this.chart.chart!.update();

    // Prevent the page from scrolling.
    event.stopPropagation();
    event.preventDefault();
  }

  ResetZoomLevel() {
    this.zoom_level = 1;
  }

  data: ChartData<'line', (number | null)[], number> = {
    datasets: [],
    labels: []
  };

  TimestampToXAxisLabel(timestamp_us: number, digit: number = 2) {
    let timestamp_ms = timestamp_us / 1000;
    // wraps around every 10 seconds.
    let v_ms = timestamp_ms % 10000;
    let v_s = v_ms / 1000;

    // Instead of using toFixed, use the following code to get floored values.
    // It's because we want to avoid showing '10.0' for 9.9 for consistent string length.
    let mul = Math.pow(10, digit);
    return (Math.floor(v_s * mul) / mul).toFixed(digit);
  }

  // Option value used only when the chart is created.
  // Updating this value after the chart is created will not have any effect.
  readonly default_options: ChartOptions<'line'> = {
    onClick: (e) => {
      this.SetPause(!this.paused);
    },

    maintainAspectRatio: false,
    responsive: true,
    layout: {
      autoPadding: false,
    },
    animation: false,
    plugins: {
      tooltip: {
        callbacks: {
          title: (items: TooltipItem<"line">[]) => {
            if (items.length == 0) {
              return '';
            }
            let item = items[0];
            return 'timestamp: ' + this.TimestampToXAxisLabel(item.parsed.x, 3) + 's';
          }
        }
      },
    },
    scales: {
      [xAxisID]: {
        type: 'linear',
        position: 'bottom',
        clip: true,
        ticks: {
          maxRotation: 90,
          minRotation: 90,
          includeBounds: true,
          autoSkip: true,
          autoSkipPadding: 0,

          callback: (timestamp_us, index, ticks) => {
            if (typeof timestamp_us != 'number') {
              return '';
            }
            return this.TimestampToXAxisLabel(timestamp_us, 1);
          }
        },
      },
      [yMmAxisID]: {
        type: 'linear',
        position: 'left',
        min: 0.0,
        max: 4.0,
        reverse: true,
      },
      [yOnOffAxisID]: {
        type: 'linear',
        reverse: true,
        min: 0.0,
        max: 1.0,
        position: 'right',
        display: false,
        grid: {
          display: false,
          drawTicks: false,
        },
      },
      [yHzAxisID]: {
        type: 'linear',
        min: 0,
        max: 1000.0,
        position: 'right',
        display: true,
        grid: {
          display: false,
          drawTicks: false,
        },
      },
    }
  };

  ClipData(min_ms: number) {
    if (this.data.labels === undefined || this.data.labels.length == 0) {
      return;
    }

    while (this.data.labels.length > 0 && this.data.labels[0] < min_ms) {
      this.data.labels.shift();
      for (let i = 0; i < this.data.datasets.length; i++) {
        this.data.datasets[i].data.shift();
      }
    }
  }

  SetPause(pause: boolean) {
    this.paused = pause;
    if (pause) {
      this.analog_switch_state_service.Stop();
    } else {
      this.analog_switch_state_service.Run();
      this.ResetZoomLevel();
    }
  }

  SetXAxisRange(min_us: number, max_us: number) {
    this.chart.chart!.options!.scales![xAxisID]!.min = min_us;
    this.chart.chart!.options!.scales![xAxisID]!.max = max_us;
  }

  constructor() {
    this.ClearData();
  }

  ClearData() {
    this.data = {
      datasets: [],
      labels: []
    };

    let order = 0;
    this.data.datasets[kIndexPressMm] = {
      data: [],
      label: 'Press(mm)',
      xAxisID: xAxisID,
      yAxisID: yMmAxisID,
      normalized: true,
      order: order++,
      pointStyle: 'circle',

      borderWidth: 1.0,
      //showLine: false,
      pointRadius: 2.0,
      pointHitRadius: 4,
    };

    this.data.datasets[kIndexButtonState] = {
      data: [],
      xAxisID: xAxisID,
      yAxisID: yOnOffAxisID,
      stepped: 'before',
      label: 'State(on/off)',
      normalized: true,
      order: order++,

      borderWidth: 3,
      pointRadius: 0,
      pointHitRadius: 0,
    };

    this.data.datasets[kIndexActuationPoint] = {
      data: [],
      xAxisID: xAxisID,
      yAxisID: yMmAxisID,
      label: 'Actuation point(mm)',
      order: order++,

      borderWidth: 2,
      pointRadius: 0,
      pointHitRadius: 0,
    };

    this.data.datasets[kIndexReleasePoint] = {
      data: [],
      xAxisID: xAxisID,
      yAxisID: yMmAxisID,
      label: 'Release point(mm)',
      order: order++,

      borderWidth: 2,
      pointRadius: 0,
      pointHitRadius: 0,
    };

    this.data.datasets[kIndexPollingRate] = {
      data: [],
      xAxisID: xAxisID,
      yAxisID: yHzAxisID,
      label: 'Polling rate(Hz)',
      order: order++,

      borderWidth: 0.5,
      pointRadius: 0,
      pointHitRadius: 4,
    };
  }

  HandleRpcResponse(res: GetAnalogSwitchStateResponse) {
    if (res.states.length > 0) {
      // Calculate polling rate
      if (res.states.length > 1) {
        let duration_us = res.states[res.states.length - 1].timestamp_us - res.states[0].timestamp_us;
        this.polling_rate = (res.states.length - 1) / (duration_us / 1000000);
      }

      for (let i = 0; i < res.states.length; i++) {
        let state = res.states[i];
        this.data.labels!.push(state.timestamp_us);
        this.data.datasets[kIndexPressMm].data.push(state.pressed_mm);
        this.data.datasets[kIndexPollingRate].data.push(this.polling_rate);
        this.data.datasets[kIndexButtonState].data.push(state.is_triggered ? 1 : 0);

        if (state.has_rapid_trigger) {
          let trigger_state = state.rapid_trigger;
          let rel = null;
          let act = null;
          if (state.is_triggered) {
            rel = trigger_state.release_point_mm;
          } else {
            act = trigger_state.actuation_point_mm;
          }
          this.data.datasets[kIndexReleasePoint].data.push(rel);
          this.data.datasets[kIndexActuationPoint].data.push(act);
        } else if (state.has_static_trigger) {
          let trigger_state = state.static_trigger;
          this.data.datasets[kIndexActuationPoint].data.push(trigger_state.actuation_point_mm);
          this.data.datasets[kIndexReleasePoint].data.push(trigger_state.release_point_mm);
        }

        this.press_mm = state.pressed_mm;
      }

      let latest_us = res.states[res.states.length - 1].timestamp_us;
      let min_us = latest_us - kDataWindowMs * 1000;
      let max_us = latest_us;

      this.ClipData(min_us);
      this.SetXAxisRange(min_us, max_us);
      this.chart.update();
    }
  }

  ngOnInit() {
    logger.debug('ButtonPreviewComponent initialized');

    //this.chart.ev

    this.subscriptions.add(this.serial_service.ConnectionChanges().subscribe((connected) => {
      this.ClearData();
    }));

    this.subscriptions.add(this.analog_switch_state_service.analog_switch_state_received.subscribe((res) => {
      this.HandleRpcResponse(res);
    }));
  }

  ngOnDestroy() {
    console.log('ButtonPreview ngOnDestroy');
    this.ClearData();
    this.analog_switch_state_service.Stop();
    this.subscriptions.unsubscribe();
  }
};
