import { Component, Input, Output, ViewChild, ElementRef, EventEmitter, NgZone, SimpleChanges, OnInit, OnChanges, OnDestroy } from '@angular/core';
import * as d3 from 'd3';
import * as moment from 'moment';
import { EmscriptenWasmComponentDirective } from './../../emscripten-wasm.component';

export const HmppDateTimeFormatting = 'YYYY-MM-DD hh:mm:ss.SSSS';
const DefaultRange: NumberRange = { from: 0.0, to: 1.0 };

@Component({
  selector: 'app-m3c1',
  templateUrl: './m3c1.component.html',
  styleUrls: ['./m3c1.component.scss']
})
export class M3c1Component extends EmscriptenWasmComponentDirective implements OnInit, OnChanges, OnDestroy {
  @ViewChild('canvas') canvas: ElementRef;
  @ViewChild('chart') chart: ElementRef;
  @Output() moduleResolvedEvent = new EventEmitter();
  @Input() axes: ChartAxes;
  @Input() toolCount: number;
  @Input() timeFormat: string;

  componentName = 'Module no. 2 - Component no. 1';

  hmDiv: any;
  axesSvg: any;

  xScale: any;
  yScale: any;

  xRange: NumberRange = { from: DefaultRange.from, to: DefaultRange.to };
  yRange: NumberRange = { from: DefaultRange.from, to: DefaultRange.to };
  zRange: NumberRange = { from: DefaultRange.from, to: DefaultRange.to };

  error: string;

  private formatTime: any;

  constructor(private ngZone: NgZone) {
    super('HmppModule', 'hmpp.js');
    this.createTimeFormatter();
  }

  private createTimeFormatter(): void {
    this.formatTime = d3.timeFormat(this.timeFormat);
  }

  private selectItems(): void {
    this.hmDiv = d3.select('div#hmpp');
    this.axesSvg = d3.select('svg#axes');
  }

  private scaleHmDiv(size: ChartSize): void {
    if (this.hmDiv) {
      this.hmDiv.style('position', 'relative')
        .style('width', (size.width + this.axes.size.width + this.axes.x) + 'px')
        .style('height', (size.height + this.axes.size.height + this.axes.y) + 'px');
    }
  }

  private scaleSvgAxes(size: ChartSize): void {
    if (this.axesSvg) {
      this.axesSvg
        .attr('width', size.width + this.axes.size.width + this.axes.x)
        .attr('height', size.height + this.axes.size.height + this.axes.y)
        .style('position', 'absolute')
        .style('left', (this.axes.x) + 'px')
        .style('top', (this.axes.y) + 'px');
    }
  }

  private positionCanvas(): void {
    d3.select('canvas')
      .style('position', 'absolute')
      .style('left', (this.axes.size.width + this.axes.x) + 'px')
      .style('top', (2 * this.axes.y) + 'px');
  }

  ngOnInit(): void {
    this.selectItems();
    this.positionCanvas();
  }

  ngOnChanges(changes: SimpleChanges): void {
    for (const change in changes) {
      if (change === 'timeFormat') {
        console.log('Update the time formatter');
        this.createTimeFormatter();
      }
    }
  }

  ngOnDestroy(): void {
    if (super.module) {
      // console.log('Calling shutdown in module');
      // super.module.ccall('shutdown', 'void', [], []);
    }
  }
}
