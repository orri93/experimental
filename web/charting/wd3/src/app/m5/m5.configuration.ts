import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { environment } from './../../environments/environment';
import { M5ModuleConfiguration } from './shared/models/m5-configuration.model';

const CONFIGURATION_ASSETS = 'm5.json';

@Injectable({
  providedIn: 'root'
})
export class M5Configuration {
  static settings: M5ModuleConfiguration;

  constructor(private http: HttpClient) {}

  load(): Promise<void> {
    const jsonFile = `${environment.configurationPath}` + CONFIGURATION_ASSETS;
    return new Promise<void>((resolve, reject) => {
      this.http.get(jsonFile).toPromise()
        .then((response: M5ModuleConfiguration) => {
          M5Configuration.settings = response as M5ModuleConfiguration;
          resolve();
        }).catch((response: any) => {
          reject(`Could not load file '${jsonFile}': ${JSON.stringify(response)}`);
        });
    });
  }
}
