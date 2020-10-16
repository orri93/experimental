import { Component, OnInit, OnDestroy, ViewChild, ElementRef } from '@angular/core';
import { catchError, map, tap } from 'rxjs/operators';
import { HttpClientDataService } from './../http-client-data.service';
import { WebSocketService } from './../web-socket.service';

@Component({
  selector: 'app-experimental',
  templateUrl: './experimental.component.html',
  styleUrls: ['./experimental.component.css']
})
export class ExperimentalComponent implements OnInit, OnDestroy {
  @ViewChild("show") show: ElementRef;

  error: string;

  constructor(
    private dataService: HttpClientDataService,
    private webSocketService: WebSocketService) {
      this.webSocketService.connect();
    }

  ngOnInit(): void {
    this.webSocketService.dataUpdates().subscribe( (data: any) => {
      console.log("Data: " + data);
    });
  }

  showMatrix(): void {
    console.log("Show Matrix Button Clicked");

    this.dataService.getMatrix(100).subscribe(m => {
      console.log(`Got Matrix: ${JSON.stringify(m)}`);

      let div = <HTMLDivElement>this.show.nativeElement;
      div.innerHTML = `Got Matrix: ${JSON.stringify(m)}`;

    });
  }

  ngOnDestroy(): void {
    this.webSocketService.close();
  }
}
