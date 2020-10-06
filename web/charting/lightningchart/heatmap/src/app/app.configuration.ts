import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { environment } from '../environments/environment';

@Injectable()
export class AppConfiguration {
  static settings: ApplicationConfiguration;

  constructor(private http: HttpClient) {}

  load() {
    const jsonFile = `${environment.configurationFile}`;
    return new Promise<void>((resolve, reject) => {
      this.http.get(jsonFile).toPromise()
        .then((response: ApplicationConfiguration) => {
          AppConfiguration.settings = <ApplicationConfiguration>response;
          resolve();
        }).catch((response: any) => {
          reject(`Could not load file '${jsonFile}': ${JSON.stringify(response)}`);
        });
    });
  }
}
