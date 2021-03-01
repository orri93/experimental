import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';

import { AngularResizedEventModule } from 'angular-resize-event';

import { MatCardModule } from '@angular/material/card';
import { MatGridListModule } from '@angular/material/grid-list';
import { MatButtonModule } from '@angular/material/button';
import { MatInputModule } from '@angular/material/input';

import { M1c1Component } from './m1c1/m1c1.component';
import { M1c2Component } from './m1c2/m1c2.component';
import { M1c3Component } from './m1c3/m1c3.component';
import { M1c4Component } from './m1c4/m1c4.component';

@NgModule({
  declarations: [
    M1c1Component,
    M1c2Component,
    M1c3Component,
    M1c4Component
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
    M1c1Component,
    M1c2Component,
    M1c3Component,
    M1c4Component
  ]
})
export class M1Module { }
