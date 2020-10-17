import { Injectable } from '@angular/core';
import { HttpClient, HttpResponse, HttpHeaders, HttpParams } from '@angular/common/http';
import { Observable, of } from 'rxjs';
import { catchError, retry } from 'rxjs/operators';

import { environment } from '../environments/environment';
import { DataService } from './data.service';
import { MessageService } from './message.service';
//import { Matrix, Vector } from './data.service.d';

@Injectable({ providedIn: 'root' })
export class HttpClientDataService extends DataService {

  constructor(private http: HttpClient, private messageService: MessageService) {
    super();
  }

  public static createGetMatrixUrl(count: number): string {
    return `${environment.restPath}/matrix/${count}`;
  }

  getMatrix(count: number): Observable<Matrix> {
    const restPath = HttpClientDataService.createGetMatrixUrl(count);
    const httHeaders = new HttpHeaders({ 'Content-Type': 'application/json' });
    const options = {
      headers: httHeaders,
      withCredentials: true,
      //params: new HttpParams().set('count', count.toString())
    };
    return this.http.get<Matrix>(restPath, options).pipe(
      retry(3), // retry a failed request up to 3 times
      catchError(this.handleError<Matrix>('getMatrix',
        super.createEmptyMatrix())));
  }

  getVector(): Observable<Vector> {
    const restPath = `${environment.restPath}/vector`;
    const httHeaders = new HttpHeaders({ 'Content-Type': 'application/json' });
    const options = {
      headers: httHeaders,
      withCredentials: true
      //params: new HttpParams().set('Authorization', 'test')
    };
    return this.http.get<Matrix>(restPath, options).pipe(
      retry(3), // retry a failed request up to 3 times
      catchError(this.handleError<Vector>('getVector', {p: []})));
  }

  /**
   * Handle Http operation that failed.
   * Let the app continue.
   * @param operation - name of the operation that failed
   * @param result - optional value to return as the observable result
   */
  private handleError<T>(operation = 'operation', result?: T): any {
    return (error: any): Observable<T> => {

      // TODO: send the error to remote logging infrastructure
      console.error(error); // log to console instead

      // TODO: better job of transforming error for user consumption
      this.log(`${operation} failed: ${error.message}`);

      // Let the app keep running by returning an empty result.
      return of(result as T);
    };
  }

  /** Log a HeroService message with the MessageService */
  private log(message: string): void {
    this.messageService.add(`HeroService: ${message}`);
  }
}
