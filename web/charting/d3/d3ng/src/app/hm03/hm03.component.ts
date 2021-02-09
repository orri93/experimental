import { Component, ViewChild, Input, ElementRef, NgZone, OnInit, OnDestroy } from '@angular/core';
import { Subscription, Observable, interval } from 'rxjs';

import * as d3 from 'd3';
import * as _ from 'lodash';

import { AppConfiguration } from './../app.configuration';
import { EmscriptenWasmComponent } from './../emscripten-wasm.component';
import { S1Service } from './../s1.service';

const requestFullscreen = S1Service.getRequestFullscreen(document);

const Duration = 10 * 60 * 1000;

const ParseDateTime = d3.timeParse('%Y-%m-%d %H:%M:%S');
const FormatTime = d3.timeFormat('%H:%M:%S');

@Component({
  selector: 'app-hm03',
  templateUrl: './hm03.component.html',
  styleUrls: ['./hm03.component.css']
})
export class Hm03Component extends EmscriptenWasmComponent implements OnDestroy, OnInit {
  @ViewChild('canvas') canvas: ElementRef;
  @ViewChild('chart') chart: ElementRef;

  size: ChartSize;
  axes: ChartAxes;

  title = 'Third Experiment';

  error: string;
  supportsFullscreen: boolean;

  hmDiv: any;
  axesSvg: any;

  xScale: any;
  yScale: any;

  xRange: RangeItem;
  yRange: RangeItem;

  private observable: Observable<number>;
  private timer: Subscription;

  private firstInterval: boolean;

  constructor(private ngZone: NgZone) {
    super('HmModule', 'hm.js');

    const self = this;
    const configuration = AppConfiguration.settings.heatmap;
    const timer = AppConfiguration.settings.timer;
    this.observable = interval(timer.interval);
    self.size = { width: configuration.size.width, height: configuration.size.height };
    self.axes = _.clone(configuration.axes);

    self.axes.size.height = 64;

    this.xRange = _.clone(configuration.range.x);
    this.yRange = _.clone(configuration.range.y);

    this.updateTimeRange(Duration);

    this.supportsFullscreen = !!requestFullscreen;

    this.moduleDecorator = (mod) => {
      mod.arguments = [
        self.size.width.toString(),
        self.size.height.toString()
      ];
      mod.preRun = [];
      mod.canvas = self.canvas.nativeElement as HTMLCanvasElement;
      mod.printErr = (what: string) => {
        if (!what.startsWith('WARNING')) {
          self.ngZone.run(() => (self.error = what));
        }
      };
    };
  }

  private updateTimeRange(duration: number): void {
    this.xRange.to = Date.now();
    this.xRange.from = this.xRange.to - duration;
  }

  private scaleItems(): void {
    const configuration = AppConfiguration.settings.heatmap;
    const axes = configuration.axes;

    this.hmDiv = d3.select('div#hm')
      .style('position', 'relative')
      .style('width', (this.size.width + axes.size.width + axes.x) + 'px')
      .style('height', (this.size.height + axes.size.height + axes.y) + 'px');

    this.axesSvg = d3.select('svg#axes')
      .attr('width', this.size.width + axes.size.width + axes.x)
      .attr('height', this.size.height + axes.size.height + axes.y)
      .style('position', 'absolute')
      .style('left', (axes.x) + 'px')
      .style('top', (axes.y) + 'px');

    d3.select('canvas')
      .style('position', 'absolute')
      .style('left', (axes.size.width + axes.x) + 'px')
      .style('top', (2 * axes.y) + 'px');
  }

  private createScaleX(): void {
    this.xScale = d3.scaleLinear()
      .domain([this.xRange.from, this.xRange.to])
      .range([0, this.size.width]);
  }

  private createScaleY(): void {
    this.yScale = d3.scaleLinear()
      .domain([this.yRange.from, this.yRange.to])
      // .range([this.size.height, 0]);
      .range([0, this.size.height]);
  }

  private drawAxesX(): void {
    const g = this.axesSvg.append('g')
      .attr('id', 'xag')
      .attr('transform', 'translate('
        + this.axes.x + ','
        + (this.size.height + this.axes.y) + ')')
      .call(d3.axisBottom(this.xScale).tickFormat(FormatTime));
    g.selectAll('text')
      .style('text-anchor', 'end')
      .attr('dx', '-.8em')
      .attr('dy', '.15em')
      .attr('transform', 'rotate(-65)');
    if (this.axes.color) {
      g.selectAll('line').attr('stroke', this.axes.color);
      g.selectAll('path').attr('stroke', this.axes.color);
      g.selectAll('text').attr('fill', this.axes.color);
    }
  }

  private drawAxesY(): void {
    const g = this.axesSvg.append('g')
      .attr('id', 'yag')
      .attr('transform', 'translate(' + this.axes.x + ', ' +  this.axes.y + ')')
      .call(d3.axisLeft(this.yScale));
    if (this.axes.color) {
      g.selectAll('line').attr('stroke', this.axes.color);
      g.selectAll('path').attr('stroke', this.axes.color);
      g.selectAll('text').attr('fill', this.axes.color);
    }
  }

  private removeAxesX(): void {
    d3.select('g#xag').remove();
  }

  private removeAxesY(): void {
    d3.select('g#yag').remove();
  }

  private createScales(): void {
    this.createScaleX();
    this.createScaleY();
  }

  private drawAxes(): void {
    this.drawAxesX();
    this.drawAxesY();
  }

  renderAxesX(): void {
    this.removeAxesX();
    this.createScaleX();
    this.drawAxesX();
  }

  renderAxesY(): void {
    this.removeAxesY();
    this.createScaleY();
    this.drawAxesY();
  }

  ngOnInit(): void {
    this.scaleItems();
    this.createScales();
    this.drawAxes();
  }

  onTimer(): void {
    if (!this.firstInterval) {
    } else {
      console.log('On timer first interval');
      this.firstInterval = false;
    }
    this.module.ccall('shift', 'void', [], []);
    d3.select('g#xag').remove();
    this.updateTimeRange(Duration);
    this.renderAxesX();
  }

  toggleFullscreen(): void {
    if (requestFullscreen) {
      requestFullscreen.bind(this.chart.nativeElement)();
    }
  }

  start(): void {
    console.log('Start');
    if (this.timer == null || this.timer.closed) {
      this.firstInterval = true;
      this.timer = this.observable
        .subscribe(( x => { this.onTimer(); }));
      console.log('Starting timer subscription.');
    } else {
      console.log('Timer subscription already started.');
    }
  }

  stop(): void {
    console.log('Stop');
    if (this.timer.closed) {
      console.log('Timer already closed.');
    } else {
      console.log('Unsubscribe the timer.');
      this.timer.unsubscribe();
    }
  }

  ngOnDestroy(): void {
    console.log('On ng destroy');
    if (this.module) {
      console.log('Calling shutdown in module');
      this.module.ccall('shutdown', 'void', [], []);
    }
  }
}
