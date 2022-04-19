import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';

import { MatSliderModule } from '@angular/material/slider';
import { MatButtonModule } from '@angular/material/button';
import { MatInputModule } from '@angular/material/input';
import { MatIconModule } from '@angular/material/icon';

import { M1Module } from './m1/m1.module';

import { RpComponent } from './rp/rp.component';
import { P1Component } from './p1/p1.component';
import { P2Component } from './p2/p2.component';
import { P0Component } from './p0/p0.component';
import { P3Component } from './p3/p3.component';

@NgModule({
  declarations: [
    AppComponent,
    RpComponent,
    P1Component,
    P2Component,
    P0Component,
    P3Component
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    BrowserAnimationsModule,
    MatSliderModule,
    MatButtonModule,
    MatInputModule,
    MatIconModule,
    M1Module
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
