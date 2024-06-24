import { Component, Input, inject } from '@angular/core';
import { AppConsts } from '../consts';

@Component({
  selector: 'app-group-icon',
  standalone: true,
  imports: [],
  template: `
  <span class="group-icon"
        [style.background]="app_consts.configGroupColor(group_id)"
  ></span>
  <span class="group-icon-content">
    <ng-content />
  </span>
  `,
  styles: `
    .group-icon {
      display: inline-block;
      width: 1em;
      height: 1em;
      margin-right: 4px;
      vertical-align: middle;
      border-radius: 50%;
      border: 1px solid;
      border-color: #999;
    }
    .group-icon-content {
      vertical-align: middle;
    }
    `
})
export class GroupIconComponent {
  @Input()
  group_id!: number;

  app_consts = inject(AppConsts);
}
