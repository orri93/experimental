import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { HomeComponent } from './home/home.component';
import { Hm01Component } from './hm01/hm01.component';

const routes: Routes = [
  { path: '', component: HomeComponent },
  {
    path: 'hm01',
    component: Hm01Component,
    data: { demo: true, name: 'Hm01' }
  }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
