import { BrowserModule } from '@angular/platform-browser';
import { NgModule, APP_INITIALIZER } from '@angular/core';

import { HttpClientModule } from "@angular/common/http";

import { AppComponent } from './app.component';
import { AppConfiguration } from './app.configuration';
import { SdlHeatmapComponent } from './sdl-heatmap/sdl-heatmap.component';

export function initializeApp(appConfiguration: AppConfiguration) {
  return () => appConfiguration.load();
}

@NgModule({
  declarations: [
    AppComponent,
    SdlHeatmapComponent
  ],
  imports: [
    BrowserModule,
    HttpClientModule
  ],
  providers: [
    AppConfiguration, {
      provide: APP_INITIALIZER,
      useFactory: initializeApp,
      deps: [AppConfiguration],
      multi:true
    }
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
