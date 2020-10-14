import { Component, OnInit, ViewChild, ElementRef } from '@angular/core';
import { HttpClientDataService } from './../http-client-data.service';

@Component({
  selector: 'app-experimental',
  templateUrl: './experimental.component.html',
  styleUrls: ['./experimental.component.css']
})
export class ExperimentalComponent implements OnInit {
  @ViewChild("show") show: ElementRef;

  error: string;

  constructor(private dataService: HttpClientDataService) { }

  ngOnInit(): void {
  }

  showMatrix(): void {
    console.log("Show Matrix Button Clicked");

    this.dataService.getMatrix().subscribe(m => {
      console.log(`Got Matrix: ${JSON.stringify(m)}`);

      let div = <HTMLDivElement>this.show.nativeElement;
      div.innerHTML = `Got Matrix: ${JSON.stringify(m)}`;

    });
  }
}
