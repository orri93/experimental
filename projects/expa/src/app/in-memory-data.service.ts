import { Injectable } from '@angular/core';
import { InMemoryDbService, RequestInfo } from 'angular-in-memory-web-api';
import { Observable } from 'rxjs';

@Injectable({ providedIn: 'root' })
export class InMemoryDataService implements InMemoryDbService {

  createDb(reqInfo?: RequestInfo): {} | Observable<{}> | Promise<{}> {

    const db = { };

    if(reqInfo) {
      console.log("Request Information URL: " + reqInfo.url);
    }

    return db;
  }
}
