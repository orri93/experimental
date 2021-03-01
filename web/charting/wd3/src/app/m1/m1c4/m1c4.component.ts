import { Component, Output, EventEmitter, ViewChild, ElementRef, AfterViewInit } from '@angular/core';
import { ResizedEvent } from 'angular-resize-event';
import { M1c2Component } from './../m1c2/m1c2.component';

@Component({
  selector: 'app-m1c4',
  templateUrl: './m1c4.component.html',
  styleUrls: ['./m1c4.component.scss']
})
export class M1c4Component /* implements AfterViewInit */ {
  @ViewChild('m1c2') m1c2: M1c2Component;
  @ViewChild('wd3div') wd3DivElement: ElementRef;
  @Output() sizeChangedEvent = new EventEmitter<ChartSize>();

  componentName = 'Module no. 1 - Component no. 4';

  size: ChartSize;

  private webAssemblyOriginalHeight: number;
  private ignoreResize = false;

  constructor() { }

  onWd3Resized(event: ResizedEvent): void {
    if (this.size) {
      if (this.size.width !== event.newWidth || this.size.height !== event.newHeight) {
        this.size.width = event.newWidth;
        this.size.height = event.newHeight;
        if (!this.ignoreResize) {
          const size: ChartSize = { width: this.size.width, height: this.webAssemblyOriginalHeight };
          // console.log('WebAssembly need resizing to size width: ' + size.width + ' and height: ' + size.height);
          this.m1c2.resizeWebAssembly(size);
          this.ignoreResize = true;
        } else {
          // console.log('Ignore WebAssembly resizing');
          this.ignoreResize = false;
        }
      }
    } else {
      this.size = { width: event.newWidth, height: event.newHeight };
      this.webAssemblyOriginalHeight = this.size.height;
      this.ignoreResize = true;
    }
    // console.log('Output size width: ' + this.size.width + ' and height: ' + this.size.height);
    this.sizeChangedEvent.emit(this.size);
  }

  // ngAfterViewInit(): void {
  //   if (this.wd3DivElement) {
  //     const wd3Div = this.wd3DivElement.nativeElement as HTMLDivElement;
  //     const size: ChartSize = { width: wd3Div.clientWidth, height: wd3Div.clientHeight };
  //     console.log('Div size width: ' + size.width + ' and height: ' + size.height);
  //     if (this.m1c2) {
  //       this.m1c2.createWebAssembly(size);
  //     }
  //   }
  // }
}
