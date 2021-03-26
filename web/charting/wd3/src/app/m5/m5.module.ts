import { NgModule, APP_INITIALIZER } from '@angular/core';
import { HttpClientModule, HttpClientXsrfModule } from '@angular/common/http';
import { CommonModule } from '@angular/common';

import { AngularResizedEventModule } from 'angular-resize-event';

import { M5Configuration } from './m5.configuration';
import { M5c1Component } from './m5c1/m5c1.component';
import { M5c2Component } from './m5c2/m5c2.component';

export function initializeModule(moduleConfiguration: M5Configuration): any {
  return (): Promise<void> => moduleConfiguration.load();
}

@NgModule({
  declarations: [
    M5c1Component,
    M5c2Component
  ],
  imports: [
    CommonModule,
    HttpClientModule,
    HttpClientXsrfModule,
    AngularResizedEventModule
  ],
  providers: [
    M5Configuration, {
      provide: APP_INITIALIZER,
      useFactory: initializeModule,
      deps: [ M5Configuration ],
      multi: true
    }
  ],
  exports: [
    M5c1Component,
    M5c2Component
  ]
})
export class M5Module { }
