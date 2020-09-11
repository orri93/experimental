import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { HmcComponent } from './hmc/hmc.component';
import { HmcChartComponent } from './hmc-chart/hmc-chart.component';

@NgModule({
  declarations: [
    AppComponent,
    HmcComponent,
    HmcChartComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
