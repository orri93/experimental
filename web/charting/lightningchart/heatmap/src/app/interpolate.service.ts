import { Injectable } from '@angular/core';

import { InterpolateData } from './interpolate-data.datamodel';
import { InterpolationPoint } from './interpolate-point.datamodel';

@Injectable({
  providedIn: 'root'
})
export class InterpolateService {

  constructor() { }

  public interpolate(data: InterpolateData): any[] {
    let pointsArray: any[] = [];
    let yIncrement = (data.yValue1 - data.yValue2) / data.resolution;
    let zIncrement = (data.zValue1 - data.zValue2) / data.resolution;
    let yCalculated = data.yValue2;
    let zCalculated = data.zValue2;

    if (yIncrement > 0) {  // Two tools cannot be at the same location
      for (let i = 0; i < data.resolution; i++) {
        yCalculated += yIncrement;
        zCalculated += zIncrement;

        if (yCalculated > data.yValue1) {
          yCalculated = data.yValue1;
        }

        pointsArray.push({ x: data.xValue, y: yCalculated, value: zCalculated });
      }
    }
    return pointsArray;
  }

}
