import { Component, ViewChild, ChangeDetectorRef } from '@angular/core';
import * as _ from 'lodash-es';
import { AppConfiguration } from './../app.configuration';
import { M2c2Component } from './../m2/m2c2/m2c2.component';

@Component({
  selector: 'app-p5',
  templateUrl: './p5.component.html',
  styleUrls: ['./p5.component.scss']
})
export class P5Component {
  @ViewChild('m2c2') m2c2: M2c2Component;

  pageName = 'Page no. 5';

  size: ChartSize;
  axes: ChartAxes;
  toolCount: number;
  timeFormat: string;

  childSize: ChartSize = { width: 0, height: 0};

  constructor(private changeDetector: ChangeDetectorRef) {
    const configuration = AppConfiguration.settings.wasm;
    this.timeFormat = configuration.formatting.time;
    this.axes = _.cloneDeep(configuration.axes);
    this.toolCount = 4;
  }

  childSizeChanged(e: ChartSize): void {
    // console.log('On Child Size Changed');
    this.childSize.width = e.width;
    this.childSize.height = e.height;
    this.changeDetector.detectChanges();
  }

  onInitialize(): void {
    this.m2c2.initialize(100);
  }
}
