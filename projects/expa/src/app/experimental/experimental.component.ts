import { Component, OnInit, ViewChild, ElementRef } from '@angular/core';
import { HttpClientDataService } from './../http-client-data.service';
import { WebSocketService } from './../web-socket.service';

@Component({
  selector: 'app-experimental',
  templateUrl: './experimental.component.html',
  styleUrls: ['./experimental.component.css']
})
export class ExperimentalComponent implements OnInit {
  @ViewChild("show") show: ElementRef;

  error: string;

  constructor(
    private dataService: HttpClientDataService,
    private webSocketService: WebSocketService) { }

  ngOnInit(): void {
    this.webSocketService.connect()
      .subscribe(evt => {
        console.log("WD event data: " + evt.data);
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
}
