import { Component, Input, ViewChild, ElementRef, NgZone, SimpleChanges, OnInit, OnChanges,  OnDestroy } from '@angular/core';

import * as d3 from 'd3';

import { EmscriptenWasmComponentDirective } from './../../emscripten-wasm.component';

import { Wd3HeatmapResult, Wd3HeatmapDomain } from './../../shared/constants/hm';

const DefaultRange: NumberRange = { from: 0.0, to: 1.0 };

@Component({
  selector: 'app-m1c1',
  templateUrl: './m1c1.component.html',
  styleUrls: ['./m1c1.component.scss']
})
export class M1c1Component extends EmscriptenWasmComponentDirective implements OnInit, OnChanges, OnDestroy {
  @ViewChild('canvas') canvas: ElementRef;
  @ViewChild('chart') chart: ElementRef;
  @Input() size: ChartSize;
  @Input() axes: ChartAxes;
  @Input() toolCount: number;
  @Input() timeFormat: string;

  componentName = 'Module no. 1 - Component no. 1';

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
    super('HmModule', 'hm.js');
    this.createTimeFormatter();
    const self = this;
    this.moduleDecorator = (mod) => {
      mod.arguments = [
        self.size.width.toString(),
        self.size.height.toString(),
        self.toolCount.toString()];
      mod.preRun = [];
      mod.canvas = this.canvas.nativeElement as HTMLCanvasElement;
      mod.printErr = (what: string) => {
        if (!what.startsWith('WARNING')) {
          this.ngZone.run(() => (self.error = what));
        }
      };
    };
  }

  private createTimeFormatter(): void {
    this.formatTime = d3.timeFormat(this.timeFormat);
  }

  private scaleItems(): void {
    this.hmDiv = d3.select('div#hm')
      .style('position', 'relative')
      .style('width', (this.size.width + this.axes.size.width + this.axes.x) + 'px')
      .style('height', (this.size.height + this.axes.size.height + this.axes.y) + 'px');

    this.axesSvg = d3.select('svg#axes')
      .attr('width', this.size.width + this.axes.size.width + this.axes.x)
      .attr('height', this.size.height + this.axes.size.height + this.axes.y)
      .style('position', 'absolute')
      .style('left', (this.axes.x) + 'px')
      .style('top', (this.axes.y) + 'px');

    d3.select('canvas')
      .style('position', 'absolute')
      .style('left', (this.axes.size.width + this.axes.x) + 'px')
      .style('top', (2 * this.axes.y) + 'px');
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
    const formatter = this.formatTime ?? d3.format('d');
    const g = this.axesSvg.append('g')
      .attr('id', 'xag')
      .attr('transform', 'translate('
        + this.axes.x + ','
        + (this.size.height + this.axes.y) + ')')
      .call(d3.axisBottom(this.xScale).tickFormat(formatter));
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

  setToolCount(toolCount: number): void {
    if (super.module) {
      const result = super.module.ccall(
        'change',
        'number',
        ['number', 'boolean'],
        [ toolCount, false ]);
      console.log(`Change module call result: ${result}`);
    }
  }

  setData(index: number, depth: number, value: number): void {
    if (super.module) {
      const result = super.module.ccall(
        'set',
        'number',
        ['number', 'number', 'number'],
        [ index, depth, value ]);
    }
  }

  checkData(): boolean {
    if (super.module) {
      const result = super.module.ccall('check', 'number', [], []);
      return result === Wd3HeatmapResult.Success;
    } else {
      return false;
    }
  }

  insertData(at: number): void {
    if (super.module) {
      super.module.ccall('insert', 'void', ['number'], [ at ]);
    }
  }

  insertLast(): void {
    if (super.module) {
      super.module.ccall('insertLast', 'void', [], []);
    }
  }

  shift(): void {
    if (super.module) {
      super.module.ccall('shift', 'void', [], []);
    }
  }

  rescale(): void {
    if (super.module) {
      super.module.ccall('rescale', 'void', [], []);
      this.yRange.from = super.module.ccall(
        'getDomain',
        'number',
        ['number'],
        [Wd3HeatmapDomain.TypeY | Wd3HeatmapDomain.From]);
      this.yRange.to = super.module.ccall(
        'getDomain',
        'number',
        ['number'],
        [Wd3HeatmapDomain.TypeY | Wd3HeatmapDomain.To]);
      this.zRange.from = super.module.ccall(
        'getDomain',
        'number',
        ['number'],
        [Wd3HeatmapDomain.TypeZ | Wd3HeatmapDomain.From]);
      this.zRange.to = super.module.ccall(
        'getDomain',
        'number',
        ['number'],
        [Wd3HeatmapDomain.TypeZ | Wd3HeatmapDomain.To]);
      this.renderAxesY();
    }
  }

  setDomainX(range: NumberRange): void {
    this.xRange.from = range.from;
    this.xRange.to = range.to;
  }

  setDomainZ(range: NumberRange): void {
    if (super.module) {
      if (range.from < this.zRange.from) {
        this.module.ccall(
          'setDomain',
          'void',
          ['number', 'number'],
          [Wd3HeatmapDomain.TypeZ | Wd3HeatmapDomain.From, range.from]);
        this.zRange.from = range.from;
      }
      if (range.to > this.zRange.to) {
        this.module.ccall(
          'setDomain',
          'void',
          ['number', 'number'],
          [Wd3HeatmapDomain.TypeZ | Wd3HeatmapDomain.To, range.to]);
        this.zRange.to = range.to;
      }
    }
  }

  getIndexX(value: number): number {
    if (this.xScale) {
      return this.xScale(value);
    } else {
      return NaN;
    }
  }

  render(separation = true): void {
    if (super.module) {
      const locking = super.module.ccall('lock', 'number', [], []);
      if (locking === Wd3HeatmapResult.Success) {
        super.module.ccall('render', 'void', [], []);
        if (separation) {
          super.module.ccall('separation', 'void', [], []);
        }
        const flip = super.module.ccall('flipUnlock', 'number', [], []);
        // console.log(`Flip unlock module call result: ${flip}`);
        // super.module.ccall('warning', 'void', ['string'], ['Warning test']);
      } else {
        console.error('Lock module call failed');
      }
    }
  }

  getScreenWidth(): number {
    if (super.module) {
      const result = super.module.ccall('getScreenWidth', 'number', [], []);
      return result;
    }
  }

  setUndefinedColor(color: string): void {
    if (super.module) {
      const result = super.module.ccall(
        'setUndefinedColor',
        'number',
        ['string'],
        [color]);
      if (result !== Wd3HeatmapResult.Success) {
        console.error('Failed to set undefined color');
      }
    }
  }

  setSeparationColor(color: string, index = -1): void {
    if (super.module) {
      const result = super.module.ccall(
        'setSeparationColor',
        'number',
        ['number', 'string'],
        [index, color]);
      if (result !== Wd3HeatmapResult.Success) {
        console.error('Failed to set separation color');
      }
    }
  }

  setStopCount(count: number): void {
    if (super.module) {
      const result = super.module.ccall(
        'setStopCount',
        'number',
        ['number', 'boolean'],
        [ count, false ]);
      console.log(`Set Stop Count module call result: ${result}`);
    }
  }

  setStopColor(color: string, index: number): void {
    if (super.module) {
      const result = super.module.ccall(
        'setStopColor',
        'number',
        ['number', 'string'],
        [ index, color ]);
      console.log(`Set Stop Color module call result: ${result}`);
    }
  }

  setStopSize(size: number, index = -1): void {
    if (super.module) {
      const result = super.module.ccall(
        'setStopSize',
        'void',
        ['number', 'number'],
        [ index, size ]);
    }
  }

  createColorGradient(): void {
    if (super.module) {
      const result = super.module.ccall('createColorGradient', 'number', [], []);
      console.log(`Create Color Gradient module call result: ${result}`);
    }
  }

  redraw(separation = true): void {
    this.renderAxesX();
    this.renderAxesY();
    this.render(separation);
  }

  fullscreen(request: any): void {
    if (request) {
      request.bind(this.chart.nativeElement)();
    }
  }

  ngOnInit(): void {
    this.scaleItems();
    this.createScales();
    this.drawAxes();
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
      console.log('Calling shutdown in module');
      super.module.ccall('shutdown', 'void', [], []);
    }
  }
}
