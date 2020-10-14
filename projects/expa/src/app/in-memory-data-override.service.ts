import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';

import {
  STATUS,
  ParsedRequestUrl,
  RequestInfo,
  RequestInfoUtilities,
  ResponseOptions,
  getStatusText } from 'angular-in-memory-web-api';
import { InMemoryDataService } from './in-memory-data.service';
import { DummyService } from './dummy.service';

@Injectable({ providedIn: 'root' })
export class InMemoryDataOverrideService extends InMemoryDataService {

  constructor(private dummyService: DummyService) {
    super();
  }

  // HTTP GET interceptor
  get(reqInfo: RequestInfo) {
    const collectionName = reqInfo.collectionName;
    console.log("GET " + collectionName);
    if (collectionName === 'matrix') {
      console.log("matrix");
      return this.getMatrix(reqInfo);
    }
    return undefined; // let the default GET handle all others
  }

  // HTTP GET interceptor handles requests for matrix
  private getMatrix(reqInfo: RequestInfo) {
    return reqInfo.utils.createResponse$(() => {
      console.log('HTTP GET override');

      const dataEncapsulation = reqInfo.utils.getConfig().dataEncapsulation;

      let matrix = this.dummyService.getMatrix();

      const options: ResponseOptions = {
        body: dataEncapsulation ? {matrix} : matrix,
        status: STATUS.OK
      }

      return this.finishOptions(options, reqInfo);
    });
  }

  
  // parseRequestUrl override
  // Do this to manipulate the request URL or the parsed result
  // into something your data store can handle.
  // This example turns a request for `/foo/heroes` into just `/heroes`.
  // It leaves other URLs untouched and forwards to the default parser.
  // It also logs the result of the default parser.
  parseRequestUrl(url: string, utils: RequestInfoUtilities): ParsedRequestUrl {
    const newUrl = url.replace(/\/foo\/heroes/, '/heroes');
    // console.log('newUrl', newUrl);
    const parsed = utils.parseRequestUrl(newUrl);
    console.log(`parseRequestUrl override of '${url}':`, parsed);
    return parsed;
  }

  /////////// helpers ///////////////
  private finishOptions(options: ResponseOptions, {headers, url}: RequestInfo) {
    options.statusText = getStatusText(options.status);
    options.headers = headers;
    options.url = url;
    return options;
  }  
}
