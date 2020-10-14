import { Injectable } from '@angular/core';
import { InterpolateService } from './interpolate.service';

@Injectable({ providedIn: 'root' })
export class LcDataService {

  constructor() { }

  public static vectorIndex(vector: Vector, row: number): number {
    for(let i = 0; i < vector.p.length - 1; i++) {
      if(row >= vector.p[i].x && row <= vector.p[i + 1].x) {
        return i;
      }
    }
    return -1;
  }

  public static dataGenerator(columns: number, rows: number, matrix: Matrix): any[] {
    let mu, x1: number;
    const result = Array.from(Array(rows)).map(() => Array(columns));
    for(let col = 0; col < columns; col++) {
      let vector: Vector = col < matrix.v.length ? matrix.v[col] : null;
      for(let row = 0; row < rows; row++) {
        let index = LcDataService.vectorIndex(vector, row);
        if(index >= 0) {
          x1 = vector.p[index].x;
          mu = (row - x1) / (vector.p[index + 1].x - x1);
          result[row][col] = InterpolateService.interpolateLinear(vector.p[index].y, vector.p[index + 1].y, mu);
        } else {
          result[row][col] = 0.0;
        } 
      }
    }
    return result;
  }
}
