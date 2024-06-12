import { Component, EventEmitter, Input, Output } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { MatSliderModule } from '@angular/material/slider';

@Component({
  selector: 'app-depth-slider',
  standalone: true,
  imports: [MatSliderModule, FormsModule],
  template: `
    <div class="slider-label-container">
      <label id="slider-name-label" class="slider-name-label">{{label}}</label>
      <label class="slider-value-label">{{value}}mm</label>
    </div>
    <mat-slider [min]="min" [max]="max" [step]="step" [disabled] = "disabled">
      <input #slider matSliderThumb  [(ngModel)]="value" >
    </mat-slider>
  `,
  styleUrl: './depth-slider.component.scss'
})
export class DepthSliderComponent {
  @Input()
  step: number = 0.1;

  @Input()
  label!: string;

  @Input()
  value!: number;

  @Input()
  min: number = 0.0;

  @Input()
  max: number = 4.0;

  @Input()
  disabled: boolean = false;
}
