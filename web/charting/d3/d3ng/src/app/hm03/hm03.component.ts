import { Component, ViewChild, ElementRef, NgZone, OnInit, OnDestroy } from '@angular/core';
import { EmscriptenWasmComponent } from '../emscripten-wasm.component';
import { interval, Subscription } from 'rxjs';
import { AppConfiguration } from '../app.configuration';
import * as d3 from 'd3';

const requestFullscreen =
  document.documentElement.requestFullscreen ||
  document.documentElement['webkitRequestFullscreen'] ||
  document.documentElement['msRequestFullscreen'] ||
  document.documentElement['mozRequestFullScreen'];

const parseDateTime = d3.timeParse('%Y-%m-%d %H:%M:%S');
const formatTime = d3.timeFormat('%H:%M:%S');

@Component({
  selector: 'app-hm03',
  templateUrl: './hm03.component.html',
  styleUrls: ['./hm03.component.css']
})
export class Hm03Component extends EmscriptenWasmComponent implements OnDestroy, OnInit {
  @ViewChild('canvas') canvas: ElementRef;
  @ViewChild('chart') chart: ElementRef;

  title = 'Third Experiment';

  error: string;
  supportsFullscreen: boolean;

  settings: ApplicationConfiguration;
  timer: Subscription;

  firstInterval: boolean;

  hmDiv: any;
  axesSvg: any;

  xScale: any;
  yScale: any;

  xRange: RangeItem;

  constructor(private ngZone: NgZone) {
    super('HmModule', 'hm.js');

    this.settings = AppConfiguration.settings;
    this.xRange = this.settings.heatmap.range.x;

    this.supportsFullscreen = !!requestFullscreen;

    const hmarg = [
      this.settings.heatmap.chart.width.toString(),
      this.settings.heatmap.chart.height.toString()
    ];

    this.moduleDecorator = (mod) => {
      mod.arguments = hmarg;
      mod.preRun = [];
      mod.canvas = <HTMLCanvasElement>this.canvas.nativeElement;
      mod.printErr = (what: string) => {
        if (!what.startsWith('WARNING')) {
          this.ngZone.run(() => (this.error = what));
        }
      };
    };
  }

  private scaleItems(): void {
    const chart = this.settings.heatmap.chart;
    const axes = this.settings.heatmap.axes;

    this.hmDiv = d3.select('div#hm')
      .style('position', 'relative')
      .style('width', (chart.width + axes.width + axes.x) + 'px')
      .style('height', (chart.height + axes.height + axes.y) + 'px');

    this.axesSvg = d3.select('svg#axes')
      .attr('width', chart.width + axes.width + axes.x)
      .attr('height', chart.height + axes.height + axes.y)
      .style('position', 'absolute')
      .style('left', (axes.x) + 'px')
      .style('top', (axes.y) + 'px');

    d3.select('canvas')
      .style('position', 'absolute')
      .style('left', (axes.width + axes.x) + 'px')
      .style('top', (2 * axes.y) + 'px');
  }

  private createScaleX(): void {
    
  }

  private createScales(): void {
    const chart = this.settings.heatmap.chart;
    const range = this.settings.heatmap.range;

    this.xScale = d3.scaleLinear()
      .domain([range.x.from, range.x.to])
      .range([0, chart.width]);

    this.yScale = d3.scaleLinear()
      .domain([range.y.from, range.y.to])
      .range([chart.height, 0]);
  }

  private drawAxes(): void {
    const chart = this.settings.heatmap.chart;
    const axes = this.settings.heatmap.axes;
    this.axesSvg.append('g')
      .attr('id', 'xag')
      .attr('transform', 'translate(' + axes.x + ',' + (chart.height + axes.y) + ')')
      .call(d3.axisBottom(this.xScale).tickFormat(d3.format('d')));
    this.axesSvg.append('g')
      .attr('id', 'yag')
      .attr('transform', 'translate(' + axes.x + ', ' +  axes.y + ')')
      .call(d3.axisLeft(this.yScale));
  }

  private redrawX(): void {
    const chart = this.settings.heatmap.chart;
    const axes = this.settings.heatmap.axes;
    this.xScale = d3.scaleLinear()
      .domain([this.xRange.from, this.xRange.to])
      .range([0, chart.width]);
    this.axesSvg.append('g')
      .attr('id', 'xag')
      .attr('transform', 'translate(' + axes.x + ',' + (chart.height + axes.y) + ')')
      .call(d3.axisBottom(this.xScale).tickFormat(d3.format('d')));
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
    this.xRange.from++;
    this.xRange.to++;
    this.redrawX();
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
      this.timer = interval(this.settings.timer.interval)
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
