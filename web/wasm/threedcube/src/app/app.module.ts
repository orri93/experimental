import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { HttpClientModule } from "@angular/common/http";

import { AppComponent } from './app.component';
import { ThreeDeeCubeComponent } from './three-dee-cube/three-dee-cube.component';

@NgModule({
  declarations: [
    AppComponent,
    ThreeDeeCubeComponent
  ],
  imports: [
    BrowserModule,
    HttpClientModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
