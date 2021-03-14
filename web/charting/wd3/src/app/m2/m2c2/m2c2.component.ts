import { Component, Input, Output, EventEmitter, ViewChild, ElementRef } from '@angular/core';
import { Subscription, Observable, interval } from 'rxjs';
import * as moment from 'moment';
import { ResizedEvent } from 'angular-resize-event';
import { M2c1Component, HmppDateTimeFormatting } from './../m2c1/m2c1.component';
import { MockDataService } from './../../shared/services/mock-data.service';
import { Point } from 'src/app/shared/models/wd3.model';

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
  private initialized = false;
  private observable: Observable<number>;
  private timer: Subscription;

  constructor(private mockDataService: MockDataService) {
    this.observable = interval(10);
  }

  private createTimer(): void {
    this.timer = this.observable.subscribe(( x => { this.onTimer(); }));
  }

  private onTimer(): void {
    let size = this.size;
    let point: Point;
    let timeFormat: string;
    let time = moment();
    let from = time.clone();

    if (this.m2c1.actualSize) {
      size = this.m2c1.actualSize;
    }

    from.subtract(1, 'minute');

    timeFormat = time.utc().format(HmppDateTimeFormatting);
    this.m2c1.startColumn(timeFormat);
    for (let i = 0; i < this.toolCount; i++) {
      point = this.mockDataService.evolve(i, 0.5, 0.1, true);
      this.m2c1.setColumn(i, point.depth, point.value);
    }
    this.m2c1.addColumn();
    timeFormat = from.utc().format(HmppDateTimeFormatting);
    this.m2c1.removeDate(timeFormat);

    this.m2c1.xRange.from = from.valueOf();
    this.m2c1.xRange.to = time.valueOf();

    this.m2c1.renderAxesX(size);
    this.m2c1.updateScaleX();
    this.m2c1.updateScaleYZ();

    const yDomain: NumberRange = this.m2c1.getScaleYDomain();
    this.m2c1.yRange.from = yDomain.from;
    this.m2c1.yRange.to = yDomain.to;
    this.m2c1.renderAxesY(size);
    this.m2c1.scaleItems(size);
    this.m2c1.render();
  }

  onModuleResolved(): void {
    console.log('On Module Resolved');
    this.initialize();
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

  initialize(count = 100): void {
    let point: Point;
    let timeFormat: string;
    let at = moment().subtract(count, 'seconds');
    let from = at.clone();
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
    this.m2c1.renderAxesX(this.size);
    this.m2c1.updateScaleX();
    this.m2c1.updateScaleYZ();
    const yDomain: NumberRange = this.m2c1.getScaleYDomain();
    this.m2c1.yRange.from = yDomain.from;
    this.m2c1.yRange.to = yDomain.to;
    this.m2c1.renderAxesY(this.size);
    this.m2c1.render();
    this.initialized = true;
  }

  start(): void {
    if (!this.initialized) {
      this.initialize();
    }
    if (this.timer) {
      if (this.timer.closed) {
        this.createTimer();
      } else {
        console.warn('Timer is still opened');
      }
    } else {
      this.createTimer();
    }
  }

  stop(): void {
    console.log('Stop clicked');
    if (this.timer) {
      if (this.timer.closed) {
        console.warn('Timer is already closed');
      } else {
        this.timer.unsubscribe();
      }
    } else {
      console.warn('The timer is still undefined');
    }
  }
}
