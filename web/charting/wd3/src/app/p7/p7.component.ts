import { Component, ViewChild } from '@angular/core';
import * as _ from 'lodash-es';
import { ResizedEvent } from 'angular-resize-event';
import { M5c2Component } from './../m5/m5c2/m5c2.component';
import { M5Service } from './../m5/shared/services/m5.service';

@Component({
  selector: 'app-p7',
  templateUrl: './p7.component.html',
  styleUrls: ['./p7.component.scss']
})
export class P7Component {
  @ViewChild('m5c2') m5c2: M5c2Component;

  pageName = 'Page no. 7';

  constructor() { }

  onWd3Resized(event: ResizedEvent): void {
    const newSize: ChartSize = M5Service.resizedEventNewSize(event);
    this.m5c2.handleSizing(newSize);
  }
}
