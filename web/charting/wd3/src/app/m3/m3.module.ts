import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { M3c1Component } from './m3c1/m3c1.component';
import { M3c2Component } from './m3c2/m3c2.component';



@NgModule({
  declarations: [M3c1Component, M3c2Component],
  imports: [
    CommonModule
  ]
})
export class M3Module { }
