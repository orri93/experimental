import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';

@Injectable({ providedIn: 'root' })
export abstract class DataService {

  abstract getMatrix(count: number): Observable<Matrix>;
  abstract getVector(): Observable<Vector>;

  protected createEmptyMatrix(): Matrix {
    return { v:[], r: {
      x: { f: 0.0, t: 0.0 },
      y: { f: 0.0, t: 0.0 } } };
  } 
}
