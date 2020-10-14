import { BrowserModule } from '@angular/platform-browser';
import { NgModule, APP_INITIALIZER } from '@angular/core';

import { HttpClientModule, HttpClientXsrfModule } from "@angular/common/http";

import { HttpClientInMemoryWebApiModule } from 'angular-in-memory-web-api';
import { InMemoryDataOverrideService } from './in-memory-data-override.service';
import { InMemoryDataService } from './in-memory-data.service';
//import { DataService } from './data.service';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { AppConfiguration } from './app.configuration';
import { HomeComponent } from './home/home.component';
import { LcComponent } from './lc/lc.component';
import { CustomComponent } from './custom/custom.component';
import { ExperimentalComponent } from './experimental/experimental.component';

export function initializeApp(appConfiguration: AppConfiguration) {
  return () => appConfiguration.load();
}

@NgModule({
  declarations: [
    AppComponent,
    HomeComponent,
    LcComponent,
    CustomComponent,
    ExperimentalComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    HttpClientModule,
    HttpClientXsrfModule,

    // The HttpClientInMemoryWebApiModule module intercepts HTTP requests
    // and returns simulated server responses.
    // Remove it when a real server is ready to receive requests.
    HttpClientInMemoryWebApiModule.forRoot(
      //InMemoryDataService,
      InMemoryDataOverrideService,
      {
        dataEncapsulation: false,
        passThruUnknownUrl: true
      }
    )
  ],
  providers: [
    //DataService,
    AppConfiguration, {
    provide: APP_INITIALIZER,
    useFactory: initializeApp,
    deps: [AppConfiguration],
    multi: true
  }],
  bootstrap: [AppComponent]
})
export class AppModule { }
