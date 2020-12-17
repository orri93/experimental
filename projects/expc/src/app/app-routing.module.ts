import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { HomeComponent } from './home/home.component';
import { BarComponent } from './bar/bar.component';
import { PieComponent } from './pie/pie.component';
import { ScatterComponent } from './scatter/scatter.component';
import { HeatmapComponent } from './heatmap/heatmap.component';

const routes: Routes = [
  { path: "", component: HomeComponent },
  {
    path: "bar",
    component: BarComponent,
    data: { demo: true, name: "Bar" }
  },
  {
    path: "pie",
    component: PieComponent,
    data: { demo: true, name: "Pie" }
  },
  {
    path: "scatter",
    component: ScatterComponent,
    data: { demo: true, name: "Scatter" }
  },
  {
    path: "heatmap",
    component: HeatmapComponent,
    data: { demo: true, name: "Heatmap" }
  }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
