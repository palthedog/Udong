import { CommonModule } from '@angular/common';
import { Component, QueryList, ViewChild, ViewChildren, inject } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { MatButtonModule } from '@angular/material/button';
import { MatCardModule } from '@angular/material/card';
import { MatRipple, MatRippleModule } from '@angular/material/core';
import { MatDividerModule } from '@angular/material/divider';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatSelectModule } from '@angular/material/select';
import { AnalogSwitchConfigComponent } from '../analog-switch-config/analog-switch-config.component';
import { BoardButtonsComponent } from '../board-buttons/board-buttons.component';
import { AnalogSwitchConfig, AnalogSwitchGroup, ButtonAssignment, ButtonId, ButtonType, DPadButtonSelector, DPadConfig, SwitchId, SwitchType, UdongConfig } from '../../proto/config';
import { AppConsts } from '../consts';
import { GroupSelectorComponent } from '../group-selector/group-selector.component';
import { SerialServiceInterface } from '../serial/serial.service';
import { logger } from '../logger';
import { SwitchIdToGroupId, compareButtonIds } from '../config_util';
import { ButtonPreviewComponent } from "../button-preview/button-preview.component";
import { Subscription } from 'rxjs';

@Component({
  selector: 'app-configurator',
  standalone: true,
  imports: [MatDividerModule, MatSelectModule, BoardButtonsComponent, MatRippleModule, MatFormFieldModule, MatCardModule, CommonModule, AnalogSwitchConfigComponent, MatButtonModule, GroupSelectorComponent, FormsModule, ButtonPreviewComponent],
  templateUrl: './configurator.component.html',
  styleUrl: './configurator.component.scss'
})
export class ConfiguratorComponent {
  serial_service = inject(SerialServiceInterface);
  consts = inject(AppConsts);

  active_switch_id: SwitchId = new SwitchId({ type: SwitchType.ANALOG_SWITCH, id: 0 });
  active_group_id: number | null = null;

  config?: UdongConfig = undefined;

  @ViewChildren(AnalogSwitchConfigComponent)
  analog_switches!: QueryList<AnalogSwitchConfigComponent>;

  @ViewChild(AnalogSwitchConfigComponent)
  analog_switch_config_?: AnalogSwitchConfigComponent;

  @ViewChild(MatRipple)
  ripple?: MatRipple;

  group_ids: Array<number> = [];

  subscriptions: Subscription = new Subscription();

  ngOnInit() {
    console.log('Configurator Component constructed');
    this.subscriptions.add(this.serial_service.ConnectionChanges().subscribe((connected) => {
      logger.info('connected', connected);
      if (connected) {
        this.serial_service.Send('get-config');
      }
    }));

    this.subscriptions.add(this.serial_service.MessageReceiveFor('get-config').subscribe((v) => {
      logger.info('get-config size: ', v.length);
      this.config = UdongConfig.deserializeBinary(v[1]);
      logger.info('Config received');
      logger.info(this.config.toObject());
      if (this.config) {
        this.group_ids = this.config.analog_switch_groups.map((group) => {
          return group.analog_switch_group_id;
        });
      } else {
        this.group_ids = [];
      }
    }));
  }

  ngOnDestroy() {
    this.subscriptions.unsubscribe();
  }

  setActiveSwitchId(switch_id: SwitchId) {
    logger.info('active switch changed', switch_id);
    this.active_switch_id = switch_id;
    this.setActiveGroupId(SwitchIdToGroupId(this.config!, this.active_switch_id));
    if (this.ripple) {
      this.ripple.launch({ centered: true });
    }
  }

  onGroupSelected() {
    logger.info('onGroupSelected');
    this.setActiveGroupId(SwitchIdToGroupId(this.config!, this.active_switch_id));
  }

  setActiveGroupId(group_id: number | null) {
    logger.info('active group changed', group_id);
    this.active_group_id = group_id;
  }

  activeAnalogSwitchConfig(): AnalogSwitchConfig | null {
    if (this.active_switch_id.type != SwitchType.ANALOG_SWITCH) {
      return null;
    }
    return this.config!.analog_switch_configs.find((aconf) => {
      return aconf.analog_switch_id == this.active_switch_id.id;
    })!;
  }

  activeAnalogSwitchGroup(): AnalogSwitchGroup | null {
    if (this.active_switch_id.type != SwitchType.ANALOG_SWITCH) {
      return null;
    }
    return this.config!.analog_switch_groups.find((group) => {
      return group.analog_switch_group_id == this.active_group_id;
    })!;
  }

  activeButtonAssignment(): ButtonAssignment {
    return this.config!.button_assignments.find((button_assignment) => {
      return button_assignment.switch_id.type == this.active_switch_id.type &&
        button_assignment.switch_id.id == this.active_switch_id.id;
    })!;
  }

  activeButtonIsLR(): boolean {
    let button = this.activeButtonAssignment();
    if (button.button_id.type != ButtonType.D_PAD) {
      return false;
    }
    return button.button_id.d_pad.direction == DPadButtonSelector.Direction.LEFT ||
      button.button_id.d_pad.direction == DPadButtonSelector.Direction.RIGHT;
  }

  activeButtonIsUD(): boolean {
    let button = this.activeButtonAssignment();
    if (button.button_id.type != ButtonType.D_PAD) {
      return false;
    }
    return button.button_id.d_pad.direction == DPadButtonSelector.Direction.UP ||
      button.button_id.d_pad.direction == DPadButtonSelector.Direction.DOWN;
  }

  lrConfigs(): [string, DPadConfig.LRSocdCleanerMode][] {
    return [
      ['Neutral', DPadConfig.LRSocdCleanerMode.LR_NEUTRAL],
      ['Last One Priority', DPadConfig.LRSocdCleanerMode.LR_LAST_PRIORITY],
    ];
  }

  get lr_socd_cleaner_mode(): DPadConfig.LRSocdCleanerMode | undefined {
    return this.config!.d_pad_config?.lr_socd_mode;
  }

  set lr_socd_cleaner_mode(value: DPadConfig.LRSocdCleanerMode | undefined) {
    if (!value) {
      return;
    }
    if (this.config!.d_pad_config == undefined) {
      this.config!.d_pad_config = new DPadConfig();
    }
    this.config!.d_pad_config.lr_socd_mode = value;
  }

  get ud_socd_cleaner_mode(): DPadConfig.UDSocdCleanerMode | undefined {
    return this.config!.d_pad_config?.ud_socd_mode;
  }

  set ud_socd_cleaner_mode(value: DPadConfig.UDSocdCleanerMode | undefined) {
    if (!value) {
      return;
    }
    if (this.config!.d_pad_config == undefined) {
      this.config!.d_pad_config = new DPadConfig();
    }
    this.config!.d_pad_config.ud_socd_mode = value;
  }

  udConfigs(): [string, DPadConfig.UDSocdCleanerMode][] {
    return [
      ['Neutral', DPadConfig.UDSocdCleanerMode.UD_NEUTRAL],
      ['Last One Priority', DPadConfig.UDSocdCleanerMode.UD_LAST_PRIORITY],
      ['Up Priority', DPadConfig.UDSocdCleanerMode.UP_PRIORITY],
      ['Down Priority', DPadConfig.UDSocdCleanerMode.DOWN_PRIORITY],
    ];
  }

  compareValue(a: ButtonId, b: ButtonId): boolean {
    return compareButtonIds(a, b);
  }

  Save() {
    if (this.config == undefined) {
      return;
    }
    logger.info('save:', this.config);
    this.serial_service.SendBinary('save-config', this.config.serializeBinary());
  }
}
