import { Component, Output, EventEmitter, ViewChild, ElementRef, AfterViewInit } from '@angular/core';
import { ResizedEvent } from 'angular-resize-event';

@Component({
  selector: 'app-m1c3',
  templateUrl: './m1c3.component.html',
  styleUrls: ['./m1c3.component.scss']
})
export class M1c3Component implements AfterViewInit {
  @ViewChild('wd3div') wd3DivElement: ElementRef;
  @Output() sizeChangedEvent = new EventEmitter<ChartSize>();

  componentName = 'Module no. 1 - Component no. 3';

  private size: ChartSize;

  constructor() {
  }

  onWd3Resized(event: ResizedEvent): void {
    this.size = { width: event.newWidth, height: event.newHeight };
    // console.log('Output size width: ' + this.size.width + ' and height: ' + this.size.height);
    this.sizeChangedEvent.emit(this.size);
  }

  ngAfterViewInit(): void {
    // if (this.wd3DivElement) {
    //   const wd3Div = this.wd3DivElement.nativeElement as HTMLDivElement;
    //   if (wd3Div) {
    //     this.size = { width: wd3Div.clientWidth, height: wd3Div.clientHeight };
    //     console.log('Output size width: ' + this.size.width + ' and height: ' + this.size.height);
    //     this.sizeChangedEvent.emit(this.size);
    //   }
    // }
  }
}
