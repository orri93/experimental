import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { PhComponent } from './ph/ph.component';
import { P1Component } from './p1/p1.component';
import { P2Component } from './p2/p2.component';
import { P3Component } from './p3/p3.component';
import { P4Component } from './p4/p4.component';
import { P5Component } from './p5/p5.component';
import { P6Component } from './p6/p6.component';
import { P7Component } from './p7/p7.component';

const routes: Routes = [
  { path: '', component: PhComponent },
  { path: 'p1', component: P1Component, data: { type: 'page', name: 'Page 1' } },
  { path: 'p2', component: P2Component, data: { type: 'page', name: 'Page 2' } },
  { path: 'p3', component: P3Component, data: { type: 'page', name: 'Page 3' } },
  { path: 'p4', component: P4Component, data: { type: 'page', name: 'Page 4' } },
  { path: 'p5', component: P5Component, data: { type: 'page', name: 'Page 5' } },
  { path: 'p6', component: P6Component, data: { type: 'page', name: 'Page 6' } },
  { path: 'p7', component: P7Component, data: { type: 'page', name: 'Page 7' } }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
