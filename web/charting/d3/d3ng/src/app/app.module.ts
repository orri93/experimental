import { BrowserModule } from '@angular/platform-browser';
import { NgModule, APP_INITIALIZER } from '@angular/core';

import { HttpClientModule, HttpClientXsrfModule } from '@angular/common/http';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { AppConfiguration } from './app.configuration';

import { HomeComponent } from './home/home.component';
import { Hm01Component } from './hm01/hm01.component';
import { Hm02Component } from './hm02/hm02.component';

export function initializeApp(appConfiguration: AppConfiguration) {
  return () => appConfiguration.load();
}

@NgModule({
  declarations: [
    AppComponent,
    HomeComponent,
    Hm01Component,
    Hm02Component
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    HttpClientModule,
    HttpClientXsrfModule
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
