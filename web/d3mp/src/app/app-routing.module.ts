import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';

import { RpComponent } from './rp/rp.component';
import { P0Component } from './p0/p0.component';
import { P1Component } from './p1/p1.component';
import { P2Component } from './p2/p2.component';
import { P3Component } from './p3/p3.component';

const routes: Routes = [
  { path: '', component: RpComponent },
  { path: 'p0', component: P0Component, data: { type: 'page', name: 'Page 0' } },
  { path: 'p1', component: P1Component, data: { type: 'page', name: 'Page 1' } },
  { path: 'p2', component: P2Component, data: { type: 'page', name: 'Page 2' } },
  { path: 'p3', component: P3Component, data: { type: 'page', name: 'Page 3' } }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
