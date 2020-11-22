import { BrowserModule } from '@angular/platform-browser';
import { NgModule, APP_INITIALIZER } from '@angular/core';

import { HttpClientModule, HttpClientXsrfModule } from "@angular/common/http";

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { AppConfiguration } from './app.configuration';

import { HomeComponent } from './home/home.component';
import { MsetComponent } from './mset/mset.component';
import { OwlComponent } from './owl/owl.component';
import { SimpleComponent } from './simple/simple.component';
import { SnakeComponent } from './snake/snake.component';
import { CubeComponent } from './cube/cube.component';

export function initializeApp(appConfiguration: AppConfiguration) {
  return () => appConfiguration.load();
}

@NgModule({
  declarations: [
    AppComponent,
    HomeComponent,
    MsetComponent,
    OwlComponent,
    SimpleComponent,
    SnakeComponent,
    CubeComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    HttpClientModule,
    HttpClientXsrfModule
  ],
  providers: [
    AppConfiguration, {
      provide: APP_INITIALIZER,
      useFactory: initializeApp,
      deps: [AppConfiguration],
      multi: true
    }
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
