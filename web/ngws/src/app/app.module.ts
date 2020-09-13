import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { GeneratedComponentComponent } from './generated-component/generated-component.component';
import { GeneratedComponentInsideFolderComponent } from './plainsight/generated-component-inside-folder/generated-component-inside-folder.component';

@NgModule({
  declarations: [
    AppComponent,
    GeneratedComponentComponent,
    GeneratedComponentInsideFolderComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
