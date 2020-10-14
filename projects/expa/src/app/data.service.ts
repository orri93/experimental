import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';

@Injectable({ providedIn: 'root' })
export abstract class DataService {

  abstract getMatrix(): Observable<Matrix>;
  abstract getVector(): Observable<Vector>;
}
