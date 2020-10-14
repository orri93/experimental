import { Injectable } from '@angular/core';

@Injectable({ providedIn: 'root' })
export class InterpolateService {

  constructor() { }

  public static interpolate(data: InterpolateData): DataPoint[] {
    let pointArray: DataPoint[] = [];

    let yi = (data.y1 - data.y2) / data.resolution;
    let zi = (data.z1 - data.z2) / data.resolution;

    let yc = data.y2;
    let zc = data.z2;

    if (yi > 0.0) {
      // Two tools cannot be at the same location
      for (let i = 0; i < data.resolution; i++) {
        yc += yi;
        zc += zi;

        if(yc > data.y1) {
          yc = data.y1;
        }

        pointArray.push({x: data.x, y: yc, z: zc})
      }
    }

    return pointArray;
  }

  public static interpolateLinear(y1: number, y2: number, mu: number): number {
    return y1 * (1.0 - mu) + y2 * mu;
  }
}
