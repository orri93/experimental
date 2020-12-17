import { BrowserModule } from '@angular/platform-browser';
import { NgModule, APP_INITIALIZER } from '@angular/core';

import { HttpClientModule, HttpClientXsrfModule } from "@angular/common/http";

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { AppConfiguration } from './app.configuration';

import { BarComponent } from './bar/bar.component';
import { PieComponent } from './pie/pie.component';
import { ScatterComponent } from './scatter/scatter.component';
import { HomeComponent } from './home/home.component';
import { HeatmapComponent } from './heatmap/heatmap.component';

export function initializeApp(appConfiguration: AppConfiguration) {
  return () => appConfiguration.load();
}

@NgModule({
  declarations: [
    AppComponent,
    BarComponent,
    PieComponent,
    ScatterComponent,
    HomeComponent,
    HeatmapComponent
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
