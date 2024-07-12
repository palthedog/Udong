import { CommonModule } from '@angular/common';
import { Component, Input, Output, inject } from '@angular/core';
import { BehaviorSubject } from 'rxjs';
import { AppConsts } from '../consts';
import { SwitchId, SwitchType, UdongConfig } from '../../proto/config';
import { SwitchIdToButtonId, SwitchIdToGroupId } from '../config_util';
import { BoardInfo, BoardInfoProvider, SwitchInfo } from '../board_info/board_info';
import { logger } from '../logger';

@Component({
  selector: 'app-board-buttons',
  standalone: true,
  imports: [CommonModule],
  templateUrl: './board-buttons.component.html',
  styleUrl: './board-buttons.component.scss'
})
export class BoardButtonsComponent {
  board_info_provider = inject(BoardInfoProvider);

  @Input()
  config!: UdongConfig;

  @Input()
  active_switch_id: SwitchId = new SwitchId({ type: SwitchType.ANALOG_SWITCH, id: 0 });

  @Output()
  activeSwitchIdChanged = new BehaviorSubject<SwitchId>(
    new SwitchId({ type: SwitchType.ANALOG_SWITCH, id: 0 }));

  app_consts = inject(AppConsts);

  get board_info(): BoardInfo {
    let board_names = this.board_info_provider.getAllBoardNames();
    return this.board_info_provider.get(board_names[0])!;
  }

  getBoardViewBox(board_info: BoardInfo): string {
    return `${board_info.left} ${board_info.top} ${board_info.width} ${board_info.height}`;
  }

  getButtonColor(switch_id: SwitchId): string {
    if (switch_id.type === SwitchType.DIGITAL_SWITCH) {
      return '#ccc';
    } else if (switch_id.type === SwitchType.ANALOG_SWITCH) {
      let group_id = SwitchIdToGroupId(this.config, switch_id);
      return this.app_consts.configGroupColor(group_id!);
    }
    logger.error('Unknown switch_id.type: ', switch_id);

    return '#fff';
  }

  getButtonName(switch_id: SwitchId): string {
    let button_id = SwitchIdToButtonId(this.config, switch_id);
    return this.app_consts.buttonName(button_id);
  }

  onClickButton(switch_id: SwitchId) {
    logger.debug('button clicked', switch_id);
    this.activeSwitchIdChanged.next(switch_id);
  }
}
