import { ApplicationConfig, provideZoneChangeDetection } from '@angular/core';
import { provideRouter } from '@angular/router';

import { provideClientHydration } from '@angular/platform-browser';
import { provideAnimations } from '@angular/platform-browser/animations';
import { provideAnimationsAsync } from '@angular/platform-browser/animations/async';
import { routes } from './app.routes';

import { environment } from '../environments/environment';
import { provideLogger } from './logger';
import { SerialServiceInterface } from './serial/serial.service';

export const appConfig: ApplicationConfig = {
  providers: [
    provideZoneChangeDetection({ eventCoalescing: true }),
    provideRouter(routes),
    provideClientHydration(),
    provideAnimationsAsync(),
    provideAnimations(), provideAnimationsAsync(), provideAnimationsAsync(),
    provideLogger(),
    { provide: SerialServiceInterface, useClass: environment.serialService },
  ]
};
