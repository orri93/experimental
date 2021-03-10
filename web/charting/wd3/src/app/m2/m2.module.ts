import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';

import { AngularResizedEventModule } from 'angular-resize-event';

import { MatCardModule } from '@angular/material/card';
import { MatGridListModule } from '@angular/material/grid-list';
import { MatButtonModule } from '@angular/material/button';
import { MatInputModule } from '@angular/material/input';

import { M2c1Component } from './m2c1/m2c1.component';
import { M2c2Component } from './m2c2/m2c2.component';

@NgModule({
  declarations: [
    M2c1Component,
    M2c2Component
  ],
  imports: [
    CommonModule,
    FormsModule,
    MatCardModule,
    MatInputModule,
    MatButtonModule,
    MatGridListModule,
    AngularResizedEventModule
  ],
  exports: [
    M2c1Component,
    M2c2Component
  ]
})
export class M2Module { }
