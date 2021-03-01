import { Component, ViewChild, OnInit, OnDestroy } from '@angular/core';
import { Subscription, Observable, interval } from 'rxjs';
import * as _ from 'lodash';
import { AppConfiguration } from './../app.configuration';
import { Point } from './../shared/models/wd3.model';
import { M1c1Component } from './../m1/m1c1/m1c1.component';

@Component({
  selector: 'app-p1',
  templateUrl: './p1.component.html',
  styleUrls: ['./p1.component.scss']
})
export class P1Component implements OnInit, OnDestroy {
  @ViewChild('m1c1') m1c1: M1c1Component;

  supportsFullscreen: boolean;

  pageName = 'Page no. 1';

  size: ChartSize;
  axes: ChartAxes;
  toolCount: number;
  timeFormat: string;

  points: Point[] = [];

  private initialized = false;
  private observable: Observable<number>;
  private timer: Subscription;

  constructor() {
    const configuration = AppConfiguration.settings.wasm;
    this.size = _.cloneDeep(configuration.size);
    this.axes = _.cloneDeep(configuration.axes);
    this.toolCount = 4;
    this.timeFormat = configuration.formatting.time;
    this.observable = interval(10);
  }

  private evolve(point: Point, valueFactor = 0.1, depthFactor = 0.1, randomDepth = false): void {
    point.depth += randomDepth ? Math.random() * depthFactor : depthFactor;
    point.value += Math.random() * valueFactor;
  }

  private onTimer(): void {
    this.m1c1.shift();
    this.m1c1.xRange.from++;
    this.m1c1.xRange.to++;
    for (let i = 0; i < this.toolCount; i++) {
      this.m1c1.setData(i, this.points[i].depth, this.points[i].value);
      this.evolve(this.points[i], 0.5, 0.1, true);
    }
    this.m1c1.insertLast();
    this.m1c1.rescale();
    this.m1c1.render(true);
    this.m1c1.renderAxesX();
  }

  private createTimer(): void {
    this.timer = this.observable.subscribe(( x => { this.onTimer(); }));
  }

  initialize(): void {
    let value = 5.0 + Math.random();
    this.points = [];
    for (let i = 0; i < this.toolCount; i++) {
      const n = i + 1;
      const point: Point = { depth: 100.0 * n, value };
      this.points.push(point);
      value += Math.random() + 10.0 * n;
    }
    const width = this.m1c1.getScreenWidth();
    // this.demoWasmD3.setToolCount(this.toolCount);
    for (let i = 0; i < width; i++) {
      for (let j = 0; j < this.toolCount; j++) {
        this.m1c1.setData(j, this.points[j].depth, this.points[j].value);
        this.evolve(this.points[j]);
      }
      this.m1c1.insertData(i);
    }
    this.m1c1.setUndefinedColor('#333333');
    this.m1c1.setSeparationColor('#b3b3b3');
    this.m1c1.xRange.from = 0.0;
    this.m1c1.xRange.to = this.size.width;
    this.m1c1.renderAxesX();
    this.m1c1.rescale();
    this.m1c1.render(true);
    this.initialized = true;
  }

  start(): void {
    console.log('Start clicked');
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

  fullscreen(request: any): void {
    this.m1c1.fullscreen(request);
  }

  ngOnInit(): void {
  }

  ngOnDestroy(): void {
    console.log('On ng destroy');
    if (this.timer && !this.timer.closed) {
      this.timer.unsubscribe();
    }
  }
}
