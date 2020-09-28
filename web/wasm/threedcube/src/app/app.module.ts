import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { HttpClientModule } from "@angular/common/http";

import { AppComponent } from './app.component';
import { ThreeDeeCubeComponent } from './three-dee-cube/three-dee-cube.component';
import { SdlCubeComponent } from './sdl-cube/sdl-cube.component';
import { SdlChartComponent } from './sdl-chart/sdl-chart.component';

@NgModule({
  declarations: [
    AppComponent,
    ThreeDeeCubeComponent,
    SdlCubeComponent,
    SdlChartComponent
  ],
  imports: [
    BrowserModule,
    HttpClientModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
