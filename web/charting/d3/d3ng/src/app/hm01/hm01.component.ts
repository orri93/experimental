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

@Component({
  selector: 'app-hm01',
  templateUrl: './hm01.component.html',
  styleUrls: ['./hm01.component.css']
})
export class Hm01Component extends EmscriptenWasmComponent implements OnDestroy, OnInit {
  @ViewChild('canvas') canvas: ElementRef;

  title = 'First Experiment';

  error: string;
  supportsFullscreen: boolean;

  settings: ApplicationConfiguration;
  timer: Subscription;

  firstInterval: boolean;

  xAxisSvg: any;
  yAxisSvg: any;

  xScale: any;
  yScale: any;

  constructor(private ngZone: NgZone) {
    super('HmModule', 'hm.js');

    this.settings = AppConfiguration.settings;

    this.supportsFullscreen = !!requestFullscreen;

    const hmarg = [
      this.settings.heatmap.size.width.toString(),
      this.settings.heatmap.size.height.toString()
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

  private resizeItems(): void {
    const size = this.settings.heatmap.size;
    const axis = { width: 16, height: 16 };

    d3.select('div#xa')
      .attr('class', 'xa')
      .style('width', (size.width + axis.width) + 'px')
      .style('height', axis.height + 'px');

    d3.select('div#ya')
    .attr('class', 'ya')
    .style('width', (size.width + axis.width) + 'px')
    .style('height', size.height + 'px');

    this.xAxisSvg = d3.select('svg#xaxis')
      .attr('width', size.width)
      .attr('height', axis.height);

    this.yAxisSvg = d3.select('svg#yaxis')
      .attr('width', axis.width)
      .attr('height', size.height);

    d3.select('svg#corner')
      .attr('width', axis.width)
      .attr('height', axis.height);
  }

  private createScales(): void {
    const size = this.settings.heatmap.size;
    const range = this.settings.heatmap.range;

    this.xScale = d3.scaleLinear()
      .domain([range.x.from, range.x.to])
      .range([0, size.width]);

    this.yScale = d3.scaleLinear()
      .domain([range.y.from, range.y.to])
      .range([size.height, 0]);
  }

  private drawAxes(): void {
    this.xAxisSvg.append('g')
      .call(d3.axisBottom(this.xScale).tickFormat(d3.format('d')));
    this.yAxisSvg.append('g')
      .call(d3.axisLeft(this.yScale));
  }

  ngOnInit(): void {
    this.resizeItems();
    this.createScales();
    this.drawAxes();
  }

  onTimer(): void {
    if (!this.firstInterval) {
      console.log('On timer interval');
    } else {
      console.log('On timer first interval');
      this.firstInterval = false;
    }
    this.module.ccall('shift', 'void', [], []);
  }

  toggleFullscreen(): void {
    if (requestFullscreen) {
      requestFullscreen.bind(this.canvas.nativeElement)();
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
