import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { CustomWsComponent } from './custom-ws/custom-ws.component';
import { CustomComponent } from './custom/custom.component';
import { HomeComponent } from "./home/home.component";
import { LcComponent } from './lc/lc.component';

const routes: Routes = [
  { path: "", component: HomeComponent },
  {
    path: "lc",
    component: LcComponent,
    data: { demo: true, name: "LightningChart" }
  },
  {
    path: "custom",
    component: CustomComponent,
    data: { demo: true, name: "Custom Chart" }
  },
  {
    path: "customws",
    component: CustomWsComponent,
    data: { demo: true, name: "Advanced Custom Chart" }
  }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
