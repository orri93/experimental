import { Component, ViewChild } from '@angular/core';
import * as _ from 'lodash-es';
import * as moment from 'moment';
import { M5c1Component } from './../m5c1/m5c1.component';
import { MockService } from './../shared/services/mock.service';
import { M5Configuration } from './../m5.configuration';
import { Point } from './../shared/models/m5.model';

@Component({
  selector: 'app-m5c2',
  templateUrl: './m5c2.component.html',
  styleUrls: ['./m5c2.component.scss']
})
export class M5c2Component {
  @ViewChild('wd3') wd3: M5c1Component;

  legend: ChartLegend;
  axes: ChartAxes;
  padding: number;
  count: number;
  timeFormat: string;

  constructor(private mockService: MockService) {
    const configuration = M5Configuration.settings;
    this.legend = _.clone(configuration.legend);
    this.axes = _.clone(configuration.axes);
    this.padding = configuration.padding;
    this.count = configuration.count;
    this.timeFormat = configuration.formatting.time;
  }

  handleSizing(size: ChartSize): void {
    this.wd3.handleSizing(size);
  }

  onModuleResolved(): void {
    console.log('On Module Resolved');
    this.initialize();
  }

  private initialize(): void {
    const configuration = M5Configuration.settings;
    console.log('Initializing');
    this.wd3.startData();
    this.mockService.reset();
    this.mockService.initialize(configuration.count);
    const at = moment().subtract(configuration.range.time, 'seconds');
    for (let i = 0; i < configuration.mock.count; i++) {
      const points: Point[] = this.mockService.next();
      this.wd3.addDataMoment(points, at);
      at.add(configuration.range.time / configuration.mock.count, 'seconds');
    }
    this.mockService.depthStep *= 0.05;
    this.mockService.depthNoise *= 0.05;
    this.wd3.sortData();
    this.wd3.redraw();
  }
}
