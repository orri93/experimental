import { Component, Output, EventEmitter, ViewChild } from '@angular/core';
import { ResizedEvent } from 'angular-resize-event';
import { SizeService } from './../../shared/services/size.service';
import { M4c1Component } from './../m4c1/m4c1.component';

@Component({
  selector: 'app-m4c2',
  templateUrl: './m4c2.component.html',
  styleUrls: ['./m4c2.component.scss']
})
export class M4c2Component {
  @ViewChild('m4c1') m4c1: M4c1Component;
  @Output() sizeChangedEvent = new EventEmitter<ChartSize>();

  componentName = 'Module no. 4 - Component no. 2';

  constructor() { }

  onWd3Resized(event: ResizedEvent): void {
    const size = SizeService.resizedEventNewSize(event);
    const actual = this.m4c1.handleSizing(size);
    this.sizeChangedEvent.emit(actual);
  }
}
