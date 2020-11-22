import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { HomeComponent } from "./home/home.component";
import { SimpleComponent } from "./simple/simple.component";
import { SnakeComponent } from "./snake/snake.component";
import { OwlComponent } from "./owl/owl.component";
import { MsetComponent } from "./mset/mset.component";
import { CubeComponent } from './cube/cube.component';

const routes: Routes = [
  { path: "", component: HomeComponent },
  {
    path: "simple",
    component: SimpleComponent,
    data: { demo: true, name: "Simple" }
  },
  {
    path: "owl",
    component: OwlComponent,
    data: {demo: true, name: "Owl" }
  },
  {
    path: "snake",
    component: SnakeComponent,
    data: {demo: true, name: "Snake" }
  },
  {
    path: "mset",
    component: MsetComponent,
    data: { demo: true, name: "Mandelbrot Set" }
  },
  {
    path: "cube",
    component: CubeComponent,
    data: {demo: true, name: "3D Cube"}
  }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
