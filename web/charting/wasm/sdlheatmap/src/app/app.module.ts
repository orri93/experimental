import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppComponent } from './app.component';
import { SdlHeatmapComponent } from './sdl-heatmap/sdl-heatmap.component';

@NgModule({
  declarations: [
    AppComponent,
    SdlHeatmapComponent
  ],
  imports: [
    BrowserModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
