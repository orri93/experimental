import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';

@Injectable({ providedIn: 'root' })
export abstract class DataService {

  abstract getMatrix(count: number): Observable<Matrix>;
  abstract getVector(): Observable<Vector>;

  public static vectorIndex(vector: Vector, row: number): number {
    for(let i = 0; i < vector.p.length - 1; i++) {
      if(row >= vector.p[i].x && row <= vector.p[i + 1].x) {
        return i;
      }
    }
    return -1;
  }

  protected createEmptyMatrix(): Matrix {
    return { v:[], r: {
      x: { f: 0.0, t: 0.0 },
      y: { f: 0.0, t: 0.0 } } };
  }
}
