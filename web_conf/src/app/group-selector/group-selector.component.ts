import { Component, EventEmitter, Input, Output, inject } from '@angular/core';

import { MatCardModule } from '@angular/material/card';
import { MatSelectModule } from '@angular/material/select';
import { AnalogSwitchAssignment } from '../config';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatOptionModule } from '@angular/material/core';
import { AppConsts } from '../consts';
import { GroupIconComponent } from "../group-icon/group-icon.component";

@Component({
  selector: 'app-group-selector',
  standalone: true,
  templateUrl: './group-selector.component.html',
  styleUrl: './group-selector.component.scss',
  imports: [MatFormFieldModule, MatOptionModule, MatCardModule, MatSelectModule, GroupIconComponent]
})
export class GroupSelectorComponent {
  @Input()
  group_ids!: Array<number>;

  // emit current group ID
  @Output()
  selectorClosed = new EventEmitter<number>();

  app_consts = inject(AppConsts);

  setGroupId(group_id: number) {
    console.log('set group id', group_id);
    this.assignment.analog_switch_group_id = group_id;
  }

  openedChange(opened: boolean) {
    console.log('opened change', opened);

    if (!opened) {
      this.selectorClosed.next(this.assignment.analog_switch_group_id);
    }
  }

  assignment_!: AnalogSwitchAssignment;

  @Input()
  set assignment(assignment: AnalogSwitchAssignment) {
    this.assignment_ = assignment;
  }
  get assignment(): AnalogSwitchAssignment {
    return this.assignment_;
  }
}
