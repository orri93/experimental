import { Injectable } from '@angular/core';
import { EMPTY, Subject, Observable, timer } from 'rxjs';
import { catchError, switchAll, retryWhen, delayWhen, tap } from 'rxjs/operators';
import { webSocket, WebSocketSubject }  from 'rxjs/webSocket';
import { environment } from '../environments/environment';

export const TYPE_START = "start";
export const TYPE_STOP = "stop";
export const TYPE_UPDATE = "update";

/* https://javascript-conference.com/blog/real-time-in-angular-a-journey-into-websocket-and-rxjs/ */
@Injectable({ providedIn: 'root' })
export class WebSocketService {
  private socket: WebSocketSubject<WsMessage>;
  private messageSubject = new Subject();

  public messages = this.messageSubject.pipe(switchAll(), catchError(e => { throw e } ));

  public connect(cfg: { reconnect: boolean } = { reconnect: false }): void {
    if(!this.socket || this.socket.closed) {
      this.socket = this.getNewWebSocket(environment.wsPath);
    }
  }

  public dataUpdates() {
    return this.socket.asObservable();
  }

  public sendMessage(msg: WsMessage) {
    this.socket.next(msg);
  }

  public close() {
    this.socket.complete();
  }

  private getNewWebSocket(url: string): WebSocketSubject<WsMessage> {
    return webSocket({
      url: url,
      closeObserver: {
        next: () => {
          console.log('[DataService]: connection closed');
          this.socket = undefined;
          this.connect({ reconnect: true });
        }
      },
    }); 
  }

  private reconnect(observable: Observable<WsMessage>): Observable<WsMessage> {
    return observable.pipe(
      retryWhen(errors => errors.pipe(
        tap(val => console.log('[Data Service] Try to reconnect', val)),
        delayWhen(_ => timer(environment.wsReconnectInterval)))));
  }
}
