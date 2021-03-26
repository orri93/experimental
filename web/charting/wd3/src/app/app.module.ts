import { BrowserModule } from '@angular/platform-browser';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { NgModule, APP_INITIALIZER, InjectionToken } from '@angular/core';
import { FormsModule } from '@angular/forms';

import { StoreModule } from '@ngrx/store';
import { EffectsModule } from '@ngrx/effects';

import { HttpClientModule, HttpClientXsrfModule } from '@angular/common/http';

import { AngularResizedEventModule } from 'angular-resize-event';

import { MatButtonModule } from '@angular/material/button';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { AppConfiguration } from './app.configuration';

import { M1Module } from './m1/m1.module';
import { M2Module } from './m2/m2.module';
import { M3Module } from './m3/m3.module';
import { M4Module } from './m4/m4.module';
import { M5Module } from './m5/m5.module';

import { PhComponent } from './ph/ph.component';
import { P1Component } from './p1/p1.component';
import { P2Component } from './p2/p2.component';
import { P3Component } from './p3/p3.component';
import { P4Component } from './p4/p4.component';
import { P5Component } from './p5/p5.component';
import { P6Component } from './p6/p6.component';
import { P7Component } from './p7/p7.component';

export function initializeApp(appConfiguration: AppConfiguration): any {
  return () => appConfiguration.load();
}

@NgModule({
  declarations: [
    AppComponent,
    PhComponent,
    P1Component,
    P2Component,
    P3Component,
    P4Component,
    P5Component,
    P6Component,
    P7Component
  ],
  imports: [
    FormsModule,
    BrowserModule,
    AppRoutingModule,
    HttpClientModule,
    HttpClientXsrfModule,
    BrowserAnimationsModule,
    StoreModule.forRoot({}, {}),
    EffectsModule.forRoot([]),
    AngularResizedEventModule,
    MatButtonModule,
    M1Module,
    M2Module,
    M3Module,
    M4Module,
    M5Module
  ],
  providers: [
    AppConfiguration, {
      provide: APP_INITIALIZER,
      useFactory: initializeApp,
      deps: [AppConfiguration],
      multi: true
    }
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
