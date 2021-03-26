import { Injectable } from '@angular/core';
import { ResizedEvent } from 'angular-resize-event';

@Injectable({
  providedIn: 'root'
})
export class M5Service {

  constructor() { }

  static isEitherZero(size: ChartSize): boolean {
    return size.width === 0 || size.height === 0;
  }

  static isEitherNonZero(size: ChartSize): boolean {
    return size.width !== 0 || size.height !== 0;
  }

  static areBothZero(size: ChartSize): boolean {
    return size.width === 0 && size.height === 0;
  }

  static areBothNonZero(size: ChartSize): boolean {
    return size.width !== 0 && size.height !== 0;
  }

  static areSameSize(sizeA: ChartSize, sizeB: ChartSize): boolean {
    return sizeA.width === sizeB.width && sizeB.height === sizeB.height;
  }

  static areNotSameSize(sizeA: ChartSize, sizeB: ChartSize): boolean {
    return sizeA.width !== sizeB.width || sizeB.height !== sizeB.height;
  }

  static clientSize(element: HTMLDivElement): ChartSize {
    return { width: element.clientWidth, height: element.clientHeight };
  }

  static offsetSize(element: HTMLDivElement): ChartSize {
    return { width: element.offsetWidth, height: element.offsetHeight };
  }

  static resizedEventNewSize(event: ResizedEvent): ChartSize {
    return { width: event.newWidth, height: event.newHeight };
  }

  static resizedEventOldSize(event: ResizedEvent): ChartSize {
    return { width: event.oldWidth, height: event.oldHeight };
  }
}
