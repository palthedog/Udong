import { ApplicationConfig, provideZoneChangeDetection } from '@angular/core';
import { provideRouter } from '@angular/router';

import { routes } from './app.routes';
import { provideClientHydration } from '@angular/platform-browser';
import { provideAnimationsAsync } from '@angular/platform-browser/animations/async';
import { provideAnimations } from '@angular/platform-browser/animations';

import { SerialService, SerialServiceInterface } from './serial.service';
import { MockSerialService } from './mock_serial.service';


export const appConfig: ApplicationConfig = {
  providers: [
    provideZoneChangeDetection({ eventCoalescing: true }),
    provideRouter(routes),
    provideClientHydration(),
    provideAnimationsAsync(),
    provideAnimations(), provideAnimationsAsync(), provideAnimationsAsync(),


    //{ provide: SerialServiceInterface, useClass: SerialService },
    { provide: SerialServiceInterface, useClass: MockSerialService },
  ]
};
