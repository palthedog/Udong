import { Component, inject, Input, ViewChild } from '@angular/core';
import { SerialServiceInterface } from '../serial/serial.service';
import { logger } from '../logger';
import { BaseChartDirective } from 'ng2-charts';
import { ChartConfiguration, ChartData } from 'chart.js';
import { AnalogSwitchGroup } from '../../proto/config';
import { GetAnalogSwitchStateRequest, GetAnalogSwitchStateResponse } from '../../proto/rpc';

const kIndexPressMm = 0;
const kIndexButtonState = 1;
const kIndexActuationPoint = 2;
const kIndexReleasePoint = 3;
const kIndexPollingRate = 4;

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

  press_mm: number = 0;
  polling_rate: number = 0;

  connected: boolean = false;
  paused: boolean = false;

  @Input()
  active_analog_switch_id: number = 0;

  @Input()
  analog_switch_group_config: AnalogSwitchGroup | null = null;

  @ViewChild(BaseChartDirective)
  chart!: BaseChartDirective;

  data: ChartData<'line', number[], number> = {
    datasets: [
    ],
    labels: []
  };

  options: ChartConfiguration['options'] = {
    onClick: (e) => {
      logger.debug('onClick', e);
      this.paused = !this.paused;
      if (!this.paused) {
        this.sendGetAnalogSwitchState();
      }
    },
    aspectRatio: 4.0,
    layout: {
      autoPadding: false,
    },
    animation: false,
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
            let timestamp_ms = timestamp_us / 1000;
            let v = timestamp_ms % 10000;  // 10s
            return (Math.floor(v / 100) / 10).toFixed(1);
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

  clipData(min_ms: number) {
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

  constructor() {
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

  ngOnInit() {
    logger.debug('ButtonPreviewComponent initialized');

    this.serial_service.ConnectionChanges().subscribe((value) => {
      this.connected = value;
      console.log('connected', value);
      if (this.connected) {
        this.sendGetAnalogSwitchState();
      }
    });

    this.serial_service.MessageReceiveFor('get-analog-switch-state').subscribe(([cmd, v]) => {
      if (!this.chart || !this.chart.chart) {
        // The component is already destroyed
        return;
      }
      let res = GetAnalogSwitchStateResponse.deserializeBinary(v);
      if (res.states.length > 0) {
        // Calculate polling rate
        if (res.states.length > 1) {
          let duration_us = res.states[res.states.length - 1].timestamp_us - res.states[0].timestamp_us;
          this.polling_rate = res.states.length / (duration_us / 1000000);
        }

        for (let i = 0; i < res.states.length; i++) {
          let state = res.states[i];
          this.data.labels!.push(state.timestamp_us);
          this.data.datasets[kIndexPressMm].data.push(state.pressed_mm);
          this.data.datasets[kIndexPollingRate].data.push(this.polling_rate);

          if (state.has_rapid_trigger) {
            let trigger_state = state.rapid_trigger;
            this.data.datasets[kIndexActuationPoint].data.push(trigger_state.actuation_point_mm);
            this.data.datasets[kIndexReleasePoint].data.push(trigger_state.release_point_mm);
          } else if (state.has_static_trigger) {
            let trigger_state = state.static_trigger;
            this.data.datasets[kIndexActuationPoint].data.push(trigger_state.actuation_point_mm);
            this.data.datasets[kIndexReleasePoint].data.push(trigger_state.release_point_mm);
          }
          this.data.datasets[kIndexButtonState].data.push(state.is_triggered ? 1 : 0);

          this.press_mm = state.pressed_mm;
        }

        let latest_us = res.states[res.states.length - 1].timestamp_us;
        let min_us = latest_us - 3000 * 1000;
        let max_us = latest_us;

        this.clipData(min_us);
        this.chart.chart!.options!.scales!['xAxis']!.min = min_us;
        this.chart.chart!.options!.scales!['xAxis']!.max = max_us;
        this.chart.update();
      }

      if (this.connected && !this.paused) {
        setTimeout(() => {
          this.sendGetAnalogSwitchState();
        }, 5);
      } else {
        logger.info('Serial port is disconnected');
      }
    });
  }

  sendGetAnalogSwitchState() {
    let request = new GetAnalogSwitchStateRequest();
    request.analog_switch_id = this.active_analog_switch_id;
    this.serial_service.SendBinary('get-analog-switch-state', request.serializeBinary());
  }
};
