import { Component, Input, Output, EventEmitter, ViewChild, ElementRef } from '@angular/core';
import * as moment from 'moment';
import { ResizedEvent } from 'angular-resize-event';
import { M2c1Component } from './../m2c1/m2c1.component';
import { MockDataService } from './../../shared/services/mock-data.service';
import { Point } from 'src/app/shared/models/wd3.model';

const HmppDateTimeFormatting = 'YYYY-MM-DD hh:mm:ss.SSSS';

@Component({
  selector: 'app-m2c2',
  templateUrl: './m2c2.component.html',
  styleUrls: ['./m2c2.component.scss']
})
export class M2c2Component {
  @ViewChild('m2c1') m2c1: M2c1Component;
  @ViewChild('wd3div') wd3DivElement: ElementRef;
  @Output() sizeChangedEvent = new EventEmitter<ChartSize>();
  @Input() size: ChartSize;
  @Input() axes: ChartAxes;
  @Input() toolCount: number;
  @Input() timeFormat: string;

  componentName = 'Module no. 2 - Component no. 2';

  private webAssemblyOriginalHeight: number;
  private ignoreResize = false;

  constructor(private mockDataService: MockDataService) {
  }

  onWd3Resized(event: ResizedEvent): void {
    if (this.size) {
      if (this.size.width !== event.newWidth || this.size.height !== event.newHeight) {
        this.size.width = event.newWidth;
        this.size.height = event.newHeight;
        if (!this.ignoreResize) {
          const size: ChartSize = { width: this.size.width, height: this.webAssemblyOriginalHeight };
          // console.log('WebAssembly need resizing to size width: ' + size.width + ' and height: ' + size.height);
          this.m2c1.resize(size);
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

  initialize(count: number): void {
    let at = moment().subtract(count, 'seconds');
    let from = at.clone();
    let timeFormat: string;
    let point: Point;
    this.mockDataService.initialize(this.toolCount);
    this.m2c1.startData(this.toolCount);
    for (let i = 0; i < count; i++) {
      timeFormat = at.utc().format(HmppDateTimeFormatting);
      this.m2c1.startColumn(timeFormat);
      for (let j = 0; j < this.toolCount; j++) {
        point = this.mockDataService.evolve(j);
        this.m2c1.setColumn(j, point.depth, point.value);
      }
      this.m2c1.addColumn();
      at.add(1, 'second');
    }
    console.log('Data range from ' +
      from.utc().format(HmppDateTimeFormatting) + ' to ' +
      at.utc().format(HmppDateTimeFormatting))
    this.m2c1.xRange.from = from.valueOf();
    this.m2c1.xRange.to = at.valueOf();
    this.m2c1.renderAxesX();
    this.m2c1.updateScaleX();
    this.m2c1.updateScaleYZ();
    this.m2c1.render();
  }
}
