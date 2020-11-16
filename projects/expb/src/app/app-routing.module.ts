import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { HomeComponent } from "./home/home.component";
import { MsetComponent } from "./mset/mset.component";

const routes: Routes = [
  { path: "", component: HomeComponent },
  {
    path: "mset",
    component: MsetComponent,
    data: { demo: true, name: "Mandelbrot Set" }
  }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
