import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { PhComponent } from './ph/ph.component';
import { P1Component } from './p1/p1.component';
import { P2Component } from './p2/p2.component';

const routes: Routes = [
  { path: '', component: PhComponent },
  { path: 'p1', component: P1Component, data: { type: 'page', name: 'Page 1' } },
  { path: 'p2', component: P2Component, data: { type: 'page', name: 'Page 2' } }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
