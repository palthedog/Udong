import { Component, EventEmitter, Input, Output } from '@angular/core';

import { MatCardModule } from '@angular/material/card';
import { MatSelectModule } from '@angular/material/select';
import { AnalogSwitchAssignment } from '../config';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatOptionModule } from '@angular/material/core';

@Component({
  selector: 'app-group-selector',
  standalone: true,
  imports: [MatFormFieldModule, MatOptionModule, MatCardModule, MatSelectModule],
  templateUrl: './group-selector.component.html',
  styleUrl: './group-selector.component.scss'
})
export class GroupSelectorComponent {
  @Input()
  group_ids!: Array<number>;

  @Output()
  assignmentChange = new EventEmitter<AnalogSwitchAssignment>();

  assignment_!: AnalogSwitchAssignment;

  get assignment() {
    return this.assignment_;
  }

  @Input()
  set assignment(assignment: AnalogSwitchAssignment) {
    this.assignment_ = assignment;
    this.assignmentChange.next(assignment);
  }
}
