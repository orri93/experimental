import { Component, Input, Output, ViewChild, ElementRef, EventEmitter, NgZone, SimpleChanges, OnInit, OnChanges, OnDestroy } from '@angular/core';
import * as d3 from 'd3';
import * as moment from 'moment';
import { EmscriptenWasmComponentDirective } from './../../emscripten-wasm.component';

export const HmppDateTimeFormatting = 'YYYY-MM-DD hh:mm:ss.SSSS';
const DefaultRange: NumberRange = { from: 0.0, to: 1.0 };

@Component({
  selector: 'app-m2c1',
  templateUrl: './m2c1.component.html',
  styleUrls: ['./m2c1.component.scss']
})
export class M2c1Component extends EmscriptenWasmComponentDirective implements OnInit, OnChanges, OnDestroy {
  @ViewChild('canvas') canvas: ElementRef;
  @ViewChild('chart') chart: ElementRef;
  @Output() moduleResolvedEvent = new EventEmitter();
  @Input() size: ChartSize;
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

  actualSize: ChartSize;

  private formatTime: any;

  constructor(private ngZone: NgZone) {
    super('HmppModule', 'hmpp.js');
    this.createTimeFormatter();
    const self = this;
    this.moduleDecorator = (mod) => {
      let args = [];
      if (self.size) {
        self.actualSize = {
          width: self.size.width,
          height: self.size.height };
        args = [
          self.size.width.toString(),
          self.size.height.toString()
        ];
      }
      mod.arguments = args;
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

  private createScaleX(size: ChartSize): void {
    this.xScale = d3.scaleLinear()
      .domain([this.xRange.from, this.xRange.to])
      .range([0, size.width]);
  }

  private createScaleY(size: ChartSize): void {
    this.yScale = d3.scaleLinear()
      .domain([this.yRange.from, this.yRange.to])
      // .range([size.height, 0]);
      .range([0, size.height]);
  }

  private drawAxesX(size: ChartSize): void {
    const formatter = this.formatTime ?? d3.format('d');
    const g = this.axesSvg.append('g')
      .attr('id', 'xag')
      .attr('transform', 'translate('
        + this.axes.x + ','
        + (size.height + this.axes.y) + ')')
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
    this.createScaleX(this.size);
    this.createScaleY(this.size);
  }

  private drawAxes(): void {
    this.drawAxesX(this.size);
    this.drawAxesY();
  }

  scaleItems(size: ChartSize): void {
    this.scaleHmDiv(size);
    this.scaleSvgAxes(size);
  }

  renderAxesX(size = this.size): void {
    this.removeAxesX();
    this.createScaleX(size);
    this.drawAxesX(size);
  }

  renderAxesY(size = this.size): void {
    this.removeAxesY();
    this.createScaleY(size);
    this.drawAxesY();
  }

  resetGradient(): void {
    if (super.module) {
      super.module.ccall('resetGradient', 'void', [], []);
    }
  }

  addGradientColorStop(color: number, size = 64): void {
    if (super.module) {
      super.module.ccall(
        'addGradientColorStop',
        'void',
        [ 'number', 'number' ],
        [ color, size ]);
    }
  }

  addGradientColorStopText(color: string, size = 64): void {
    if (super.module) {
      const result = super.module.ccall(
        'addGradientColorStopText',
        'boolean',
        [ 'string', 'number' ],
        [ color, size ]);
      if (!result) {
        console.error('Adding Gradient Color Text Stop failed');
      }
    }
  }

  stockGradient(size = 64): void {
    if (super.module) {
      super.module.ccall('stockGradient', 'void', [ 'number' ], [ size ]);
    }
  }

  createGradient(): void {
    if (super.module) {
      super.module.ccall('createGradient', 'void', [], []);
    }
  }

  startData(size: number): void {
    if (super.module) {
      const result = super.module.ccall('startData', 'boolean', [ 'number' ], [ size ]);
      if (!result) {
        console.error('Starting Data failed');
      }
    }
  }

  removeDate(time: string): void {
    if (super.module) {
      const result = super.module.ccall('removeDate', 'boolean', [ 'string' ], [ time ]);
      if (!result) {
        console.error('Removing Data failed');
      }
    }
  }

  startColumn(time: string): void {
    if (super.module) {
      const result = super.module.ccall('startColumn', 'boolean', [ 'string' ], [ time ]);
      if (!result) {
        console.error('Starting Column failed');
      }
    }
  }

  startColumnNow(): void {
    if (super.module) {
      const result = super.module.ccall('startColumnNow', 'boolean', [ ], [ ]);
      if (!result) {
        console.error('Starting Column Now failed');
      }
    }
  }

  setColumn(index: number, depth: number, value: number): void {
    if (super.module) {
      const result = super.module.ccall(
        'setColumn',
        'boolean',
        [ 'number', 'number', 'number' ],
        [ index, depth, value ]);
      if (!result) {
        console.error('Set Column failed');
      }
    }
  }

  addColumn(): void {
    if (super.module) {
      const result = super.module.ccall('addColumn', 'boolean', [ ], [ ]);
      if (!result) {
        console.error('Starting Column Now failed');
      }
    }
  }

  updateScaleX(): void {
    if (super.module) {
      const result = super.module.ccall('updateScaleX', 'boolean', [ ], [ ]);
      if (!result) {
        console.error('Updating X Scale failed');
      }
    }
  }

  updateScaleY(): void {
    if (super.module) {
      const result = super.module.ccall('updateScaleY', 'boolean', [ ], [ ]);
      if (!result) {
        console.error('Updating Y Scale failed');
      }
    }
  }

  updateScaleYZ(): void {
    if (super.module) {
      const result = super.module.ccall('updateScaleYZ', 'boolean', [ ], [ ]);
      if (!result) {
        console.error('Updating Y and Z Scales failed');
      }
    }
  }

  setScaleZ(from: number, to: number): void {
    if (super.module) {
      const result = super.module.ccall(
        'setScaleZ',
        'boolean',
        [ 'number', 'number' ],
        [ from, to ]);
      if (!result) {
        console.error('Set Z Scale failed');
      }
    }
  }

  getScaleXDomain(): NumberRange {
    let result: NumberRange = { from: 0, to: 0 };
    if (super.module) {
      const from = super.module.ccall('getScaleXDomainFrom', 'string', [], []);
      const to = super.module.ccall('getScaleXDomainTo', 'string', [], []);
      result.from = moment(from, HmppDateTimeFormatting).local().valueOf();
      result.to = moment(to, HmppDateTimeFormatting).local().valueOf();
    }
    return result;
  }

  getScaleYDomain(): NumberRange {
    let result: NumberRange = { from: 0, to: 0 };
    if (super.module) {
      result.from = super.module.ccall('getScaleYDomainFrom', 'number', [], []);
      result.to = super.module.ccall('getScaleYDomainTo', 'number', [], []);
    }
    return result;
  }

  getScaleZDomain(): NumberRange {
    let result: NumberRange = { from: 0, to: 0 };
    if (super.module) {
      result.from = super.module.ccall('getScaleZDomainFrom', 'number', [], []);
      result.to = super.module.ccall('getScaleZDomainTo', 'number', [], []);
    }
    return result;
  }

  render(): void {
    if (super.module) {
      const result = super.module.ccall('render', 'boolean', [ ], [ ]);
      if (!result) {
        console.error('Rendering failed');
      }
    }
  }

  clear(value = 0): void {
    if (super.module) {
      const result = super.module.ccall('clear', 'boolean', [ 'number' ], [ value ]);
      if (!result) {
        console.error('Clearing failed');
      }
    }
  }

  resize(newSize: ChartSize, value = 0): void {
    this.scaleItems(newSize);
    this.renderAxesX(newSize);
    this.renderAxesY();
    if (super.module) {
      // console.log('Resizing the WebAssembly to width: ' + newSize.width + ' and height: ' + newSize.height);
      super.module.ccall(
        'resize',
        'void',
        [ 'number', 'number', 'number' ],
        [ newSize.width, newSize.height, value ]);
      if (this.actualSize) {
        this.actualSize.width = newSize.width;
        this.actualSize.height = newSize.height;
      }
    }
  }

  protected moduleResolved(): void {
    console.log('The WebAssembly module has been resolved');
    this.scaleItems(this.size);
    this.createScales();
    this.drawAxes();
    this.clear();
    this.moduleResolvedEvent.emit();
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
