import { Component, Input, Output, ViewChild, ElementRef, EventEmitter, NgZone, SimpleChanges, OnChanges } from '@angular/core';
import * as d3 from 'd3';
import * as _ from 'lodash-es';
import * as moment from 'moment';
import { loadScript } from './../../tools';
import { Point } from './../../shared/models/wd3.model';
import { DebugService } from './../shared/services/debug.service';
import { M5Service } from './../shared/services/m5.service';
import * as wasmCacheBusting from './../../../wasm-cache-busting.json';
import { environment } from './../../../environments/environment';

type EmscriptenModuleDecorator<M extends EmscriptenModule> = (module: M) => void;
const noopModuleDecorator = (mod: EmscriptenModule) => mod;

const MODULE_EXPORT_NAME = 'HmppModule';
const WASM_JAVA_SCRIPT_LOADER = 'hmpp.js';

const DefaultRange: NumberRange = { from: 0.0, to: 1.0 };

export const HmppDateTimeFormatting = 'YYYY-MM-DD hh:mm:ss.SSSS';

@Component({
  selector: 'app-m5c1',
  templateUrl: './m5c1.component.html',
  styleUrls: ['./m5c1.component.scss']
})
export class M5c1Component<M extends EmscriptenModule = EmscriptenModule> implements OnChanges {
  @ViewChild('canvaswd3') canvas: ElementRef;
  @ViewChild('wd3chart') chart: ElementRef;
  @Output() moduleResolvedEvent = new EventEmitter();
  @Input() legend: ChartLegend;
  @Input() axes: ChartAxes;
  @Input() padding: number;
  @Input() toolCount: number;
  @Input() timeFormat: string;

  hmDiv: any;
  axesSvg: any;
  wd3Canvas: any;

  xScale: any;
  yScale: any;
  zScale: any;

  xRange: NumberRange = { from: DefaultRange.from, to: DefaultRange.to };
  yRange: NumberRange = { from: DefaultRange.from, to: DefaultRange.to };
  zRange: NumberRange = { from: DefaultRange.from, to: DefaultRange.to };

  error: string;

  private module: M;
  private formatTime: any;
  private moduleDecorator: EmscriptenModuleDecorator<M>;
  private originalHeight: number;
  private wasmSize: ChartSize;
  private size: ChartSize;

  constructor(private ngZone: NgZone) {
    this.d3CreateTimeFormatter();
  }

  /*
   *  WebAssembly Module
   */
  private static calculateWebAssemblySize(size: ChartSize, axes: ChartAxes, padding: number): ChartSize {
    return {
      width: size.width - axes.size.width - axes.x - padding,
      height: size.height - axes.size.height - axes.y
    };
  }

  handleSizing(size: ChartSize): void {
    if (this.module) {
      size.height = this.originalHeight;
      if (M5Service.areNotSameSize(size, this.size)) {
        this.resize(size);
      }
    } else {
      DebugService.consoleLogSize(size, 'WD3 Handling sizing for the first time');
      this.size = _.clone(size);
      this.originalHeight = this.size.height;
      if (this.axes) {
        this.wasmSize = M5c1Component.calculateWebAssemblySize(
          this.size,
          this.axes,
          this.padding);
        const args = [
          this.wasmSize.width.toString(),
          this.wasmSize.height.toString()
        ];
        const self = this;
        this.moduleDecorator = (mod) => {
          mod.arguments = args;
          mod.preRun = [];
          mod.canvas = self.canvas.nativeElement as HTMLCanvasElement;
          mod.printErr = (what: string) => {
            if (!what.startsWith('WARNING')) {
              self.ngZone.run(() => (self.error = what));
            }
          };
        };
        this.resolveModule();
      } else {
        throw Error('WD3 axes are undefined!');
      }
    }
  }

  private resize(newSize: ChartSize, value = 0): void {
    this.size = _.clone(newSize);
    this.wasmSize = M5c1Component.calculateWebAssemblySize(
      this.size,
      this.axes,
      this.padding);
    this.scaleItems(this.size);
    this.renderAxesX(this.wasmSize);
    this.renderAxesY();
    this.renderAxesZ();
    this.renderLegend();
    this.wasmResize(this.wasmSize, value);
  }

  private resolveModule(): void {
    const self = this;
    console.log('Resolving WebAssembly module ' + MODULE_EXPORT_NAME
      + ' loaded from ' + WASM_JAVA_SCRIPT_LOADER);
    const jsVersion = wasmCacheBusting[WASM_JAVA_SCRIPT_LOADER]
      ? `?v=${wasmCacheBusting[WASM_JAVA_SCRIPT_LOADER]}`
      : '';
    loadScript(
      MODULE_EXPORT_NAME,
      `${environment.wasmAssetsPath}/${WASM_JAVA_SCRIPT_LOADER}${jsVersion}`)
      .then(() => {
        const module = {
          locateFile: (file: string) => {
            const fileVersion = wasmCacheBusting[file] ?
            `?v=${wasmCacheBusting[file]}` : '';
            return `${environment.wasmAssetsPath}/${file}${fileVersion}`;
          },
        } as M;
        const moduleDecorator: EmscriptenModuleDecorator<M> =
        self.moduleDecorator || noopModuleDecorator;
        moduleDecorator(module);
        return window[MODULE_EXPORT_NAME](module);
      })
      .then((mod) => {
        self.module = mod;
        self.moduleResolved();
      });
  }

  protected moduleResolved(): void {
    console.log('The WebAssembly module has been resolved');
    this.d3SelectItems();
    this.positionCanvas();
    this.scaleItems(this.size);
    this.createScales();
    this.moduleResolvedEvent.emit();
  }

  /*
   *  D3 formatter and selection
   */
  private d3CreateTimeFormatter(): void {
    this.formatTime = d3.timeFormat(this.timeFormat);
  }

  private d3SelectItems(): void {
    this.hmDiv = d3.select('div#wd3-hm');
    this.axesSvg = d3.select('svg#wd3-axes');
    this.wd3Canvas = d3.select('canvas.wd3-canvas');
  }

  /*
   *  Scale
   */
  private scaleHmDiv(size: ChartSize): void {
    if (this.hmDiv) {
      this.hmDiv.style('position', 'relative')
        .style('width', (size.width) + 'px')
        .style('height', (size.height) + 'px');
    } else {
      throw Error('WD3 the heatmap div is undefined!');
    }
  }

  private scaleSvgAxes(size: ChartSize): void {
    if (this.axesSvg) {
      this.axesSvg
        .attr('width', size.width)
        .attr('height', size.height)
        .style('position', 'absolute')
        .style('left', (this.axes.x) + 'px')
        .style('top', (this.axes.y) + 'px');
    } else {
      throw Error('WD3 the axes svg is undefined!');
    }
  }

  /*
   *  Position
   */
  private positionCanvas(): void {
    if (this.wd3Canvas) {
      this.wd3Canvas
        .style('position', 'absolute')
        .style('left', (this.axes.size.width + this.axes.x) + 'px')
        .style('top', (2 * this.axes.y) + 'px');
    } else {
      throw Error('WD3 the canvas is undefined!');
    }
  }

  /*
   *  Create Scales
   */
  private createScaleX(wasmSize: ChartSize): void {
    this.xScale = d3.scaleLinear()
      .domain([this.xRange.from, this.xRange.to])
      .range([0, wasmSize.width]);
  }

  private createScaleY(wasmSize: ChartSize): void {
    this.yScale = d3.scaleLinear()
      .domain([this.yRange.from, this.yRange.to])
      // .range([size.height, 0]);
      .range([0, wasmSize.height]);
  }

  private createScaleZ(wasmSize: ChartSize): void {
    this.zScale = d3.scaleLinear()
      .domain([this.zRange.from, this.zRange.to])
      .range([wasmSize.height, 0]);
  }

  /*
   *  Drawing
   */
  private drawAxesX(wasmSize: ChartSize): void {
    const formatter = this.formatTime ?? d3.format('d');
    const g = this.axesSvg.append('g')
      .attr('id', 'xag')
      .attr('transform', 'translate('
        + this.axes.x + ','
        + (wasmSize.height + this.axes.y) + ')')
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

  private drawAxesZ(wasmSize: ChartSize): void {
    const x = this.axes.x + wasmSize.width + this.legend.margin + this.legend.width;
    const g = this.axesSvg.append('g')
      .attr('id', 'zag')
      .attr('transform', 'translate(' + x + ', ' +  this.axes.y + ')')
      .call(d3.axisRight(this.zScale));
    if (this.axes.color) {
      g.selectAll('line').attr('stroke', this.axes.color);
      g.selectAll('path').attr('stroke', this.axes.color);
      g.selectAll('text').attr('fill', this.axes.color);
    }
  }

  private drawLegend(wasmSize: ChartSize): void {
    const x = this.axes.x + wasmSize.width + this.legend.margin;
    const g = this.axesSvg.append('g')
      .attr('id', 'zlg')
      .attr('transform', 'translate(' + x + ', ' +  this.axes.y + ')');
    for (let y = 0; y < wasmSize.height; y++) {
      const z = this.zScale.invert(y);
      const color = this.wasmGetZedColor(z);
      const line = g.append('line')
        .attr('x1', 0)
        .attr('y1', y)
        .attr('x2', this.legend.width)
        .attr('y2', y)
        .attr('stroke', color);
    }
  }

  private removeAxesX(): void {
    d3.select('g#xag').remove();
  }

  private removeAxesY(): void {
    d3.select('g#yag').remove();
  }

  private removeAxesZ(): void {
    d3.select('g#zag').remove();
  }

  private removeLegend(): void {
    d3.select('g#zlg').remove();
  }

  private createScales(): void {
    this.createScaleX(this.wasmSize);
    this.createScaleY(this.wasmSize);
    this.createScaleZ(this.wasmSize);
  }

  private drawAxes(): void {
    this.drawAxesX(this.wasmSize);
    this.drawAxesY();
  }

  scaleItems(size = this.size): void {
    this.scaleHmDiv(size);
    this.scaleSvgAxes(size);
  }

  renderAxesX(wasmSize = this.wasmSize): void {
    this.removeAxesX();
    this.createScaleX(wasmSize);
    this.drawAxesX(wasmSize);
  }

  renderAxesY(wasmSize = this.wasmSize): void {
    this.removeAxesY();
    this.createScaleY(wasmSize);
    this.drawAxesY();
  }

  renderAxesZ(wasmSize = this.wasmSize): void {
    this.removeAxesZ();
    this.createScaleZ(wasmSize);
    this.drawAxesZ(wasmSize);
  }

  renderLegend(wasmSize = this.wasmSize): void {
    this.removeLegend();
    this.drawLegend(wasmSize);
  }

  /*
   *  Color
   */
  setUndefinedColor(color: string): void {
    this.wasmSetUndefinedColorText(color);
  }

  /*
   *  Data
   */
  startData(toolCount = this.toolCount): void {
    this.wasmStartData(toolCount);
  }

  addDataMoment(points: Point[], time: moment.Moment): void {
    const text = time.format(HmppDateTimeFormatting);
    const length = Math.min(points.length, this.toolCount);
    this.wasmStartColumn(text);
    for (let i = 0; i < length; i++) {
      this.wasmSetColumn(i, points[i].depth, points[i].value);
    }
    this.wasmSortColumn();
    this.wasmAddColumn();
  }

  addDataMomentUtc(points: Point[], time: moment.Moment): void {
    this.addDataMoment(points, time.utc());
  }

  addDataTime(points: Point[], time: number): void {
    this.addDataMoment(points, moment(time));
  }

  addDataTimeUtc(points: Point[], time: number): void {
    this.addDataMomentUtc(points, moment(time));
  }

  removeData(time: number): void {
    const text = moment(time).utc().format(HmppDateTimeFormatting);
    this.wasmRemoveDate(text);
  }

  sortData(): void {
    this.wasmSortDate();
  }

  /*
   *  Render
   */
  redraw(): void {
    // console.log('WD3 Redraw');
    this.wasmUpdateScaleX();
    this.wasmUpdateScaleYZ();
    const xDomain: NumberRange = this.wasmGetScaleXDomain();
    this.xRange = _.clone(xDomain);
    const yDomain: NumberRange = this.wasmGetScaleYDomain();
    this.yRange = _.clone(yDomain);
    const zDomain: NumberRange = this.wasmGetScaleZDomain();
    this.zRange = _.clone(zDomain);
    this.renderAxesY();
    this.renderAxesX();
    this.renderAxesZ();
    this.renderLegend();
    this.scaleItems();
    this.wasmRender();
  }

  /*
   *  Debug
   */
  debugReport(): void {
    this.wasmDebugReport();
  }

  /*
   *  Gradient interface
   */
  private wasmResetGradient(): void {
    if (this.module) {
      this.module.ccall('resetGradient', 'void', [], []);
    }
  }

  private wasmAddGradientColorStop(color: number, size = 64): void {
    if (this.module) {
      this.module.ccall(
        'addGradientColorStop',
        'void',
        [ 'number', 'number' ],
        [ color, size ]);
    }
  }

  private wasmAddGradientColorStopText(color: string, size = 64): void {
    if (this.module) {
      const result = this.module.ccall(
        'addGradientColorStopText',
        'boolean',
        [ 'string', 'number' ],
        [ color, size ]);
      if (!result) {
        console.error('Adding Gradient Color Text Stop failed');
      }
    }
  }

  private wasmStockGradient(size = 64): void {
    if (this.module) {
      this.module.ccall('stockGradient', 'void', [ 'number' ], [ size ]);
    }
  }

  private wasmCreateGradient(): void {
    if (this.module) {
      this.module.ccall('createGradient', 'void', [], []);
    }
  }

  /*
   *  Color interface
   */
  private wasmSetUndefinedColor(color: number): void {
    if (this.module) {
      this.module.ccall('setUndefinedColor', 'void', [ 'number' ], [ color ]);
    }
  }

  private wasmSetUndefinedColorText(color: string): void {
    if (this.module) {
      this.module.ccall('setUndefinedColorText', 'void', [ 'string' ], [ color ]);
    }
  }

  private wasmGetZedColor(value: number, defaultColor = '#000000'): string {
    let result: string;
    if (this.module) {
      result = this.module.ccall(
        'getZedColor',
        'string',
        [ 'number', 'string' ],
        [ value, defaultColor ]);
    }
    return result;
  }

  /*
   *  Data interface
   */
  private wasmStartData(size: number): void {
    if (this.module) {
      const result = this.module.ccall('startData', 'boolean', [ 'number' ], [ size ]);
      if (!result) {
        console.error('Starting Data failed');
      }
    }
  }

  private wasmRemoveDate(time: string): void {
    if (this.module) {
      const result = this.module.ccall('removeDate', 'boolean', [ 'string' ], [ time ]);
      if (!result) {
        console.error('Removing Data failed');
      }
    }
  }

  private wasmSortDate(): void {
    if (this.module) {
      const result = this.module.ccall('sortData', 'void', [ ], [ ]);
    }
  }

  /*
   *  Column interface
   */
  private wasmStartColumn(time: string): void {
    if (this.module) {
      const result = this.module.ccall('startColumn', 'boolean', [ 'string' ], [ time ]);
      if (!result) {
        console.error('Starting Column failed');
      }
    }
  }

  private wasmStartColumnNow(): void {
    if (this.module) {
      const result = this.module.ccall('startColumnNow', 'boolean', [ ], [ ]);
      if (!result) {
        console.error('Starting Column Now failed');
      }
    }
  }

  private wasmSetColumn(index: number, depth: number, value: number): void {
    if (this.module) {
      const result = this.module.ccall(
        'setColumn',
        'boolean',
        [ 'number', 'number', 'number' ],
        [ index, depth, value ]);
      if (!result) {
        console.error('Set Column failed');
      }
    }
  }

  private wasmAddColumn(): void {
    if (this.module) {
      const result = this.module.ccall('addColumn', 'boolean', [ ], [ ]);
      if (!result) {
        console.error('Starting Column Now failed');
      }
    }
  }

  private wasmSortColumn(): void {
    if (this.module) {
      this.module.ccall('sortColumn', 'void', [ ], [ ]);
    }
  }

  /*
   *  Scale interface
   */
  private wasmUpdateScaleX(): void {
    if (this.module) {
      const result = this.module.ccall('updateScaleX', 'boolean', [ ], [ ]);
      if (!result) {
        console.error('Updating X Scale failed');
      }
    }
  }

  private wasmUpdateScaleY(): void {
    if (this.module) {
      const result = this.module.ccall('updateScaleY', 'boolean', [ ], [ ]);
      if (!result) {
        console.error('Updating Y Scale failed');
      }
    }
  }

  private wasmUpdateScaleYZ(): void {
    if (this.module) {
      const result = this.module.ccall('updateScaleYZ', 'boolean', [ ], [ ]);
      if (!result) {
        console.error('Updating Y and Z Scales failed');
      }
    }
  }

  private wasmSetScaleZ(from: number, to: number): void {
    if (this.module) {
      const result = this.module.ccall(
        'setScaleZ',
        'boolean',
        [ 'number', 'number' ],
        [ from, to ]);
      if (!result) {
        console.error('Set Z Scale failed');
      }
    }
  }

  private wasmGetScaleXDomain(): NumberRange {
    const result: NumberRange = { from: 0, to: 0 };
    if (this.module) {
      const from = this.module.ccall('getScaleXDomainFrom', 'string', [], []);
      const to = this.module.ccall('getScaleXDomainTo', 'string', [], []);
      result.from = moment(from, HmppDateTimeFormatting).local().valueOf();
      result.to = moment(to, HmppDateTimeFormatting).local().valueOf();
    }
    return result;
  }

  private wasmGetScaleYDomain(): NumberRange {
    const result: NumberRange = { from: 0, to: 0 };
    if (this.module) {
      result.from = this.module.ccall('getScaleYDomainFrom', 'number', [], []);
      result.to = this.module.ccall('getScaleYDomainTo', 'number', [], []);
    }
    return result;
  }

  private wasmGetScaleZDomain(): NumberRange {
    const result: NumberRange = { from: 0, to: 0 };
    if (this.module) {
      result.from = this.module.ccall('getScaleZDomainFrom', 'number', [], []);
      result.to = this.module.ccall('getScaleZDomainTo', 'number', [], []);
    }
    return result;
  }

  /*
   *  Render interface
   */
  private wasmRender(): void {
    if (this.module) {
      const result = this.module.ccall('render', 'boolean', [ ], [ ]);
      if (!result) {
        console.error('Rendering failed');
      }
    }
  }

  private wasmClear(value = 0): void {
    if (this.module) {
      const result = this.module.ccall('clear', 'boolean', [ 'number' ], [ value ]);
      if (!result) {
        console.error('Clearing failed');
      }
    }
  }

  private wasmResize(size: ChartSize, value = 0): void {
    if (this.module) {
      this.module.ccall(
        'resize',
        'void',
        [ 'number', 'number', 'number' ],
        [ size.width, size.height, value ]);
    }
  }

  /*
   *  Debug interface
   */
  private wasmDebugReport(): void {
    if (this.module) {
        this.module.ccall('debugReport', 'void', [ ], [ ]);
    }
  }

  /*
   *  Angular implementations
   */
  ngOnChanges(changes: SimpleChanges): void {
    for (const change in changes) {
      if (change === 'timeFormat') {
        console.log('Update the time formatter');
        this.d3CreateTimeFormatter();
      }
    }
  }
}
