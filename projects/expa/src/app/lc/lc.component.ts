import { Component, ViewChild, ElementRef, AfterViewInit, OnDestroy } from '@angular/core';
import { Subscription } from 'rxjs';
import {
  LUT,
  Axis,
  Point,
  Matrix,
  LUTStep,
  ChartXY,
  ChartXYOptions,
  lightningChart,
  IntensityGridSeries,
  AxisTickStrategies,
  DateTimeTickStrategy,
  NumericTickStrategy,
  TickStrategyStyler,
  TickStrategyParameters,
  EngineOptions,
  PalettedFill,
  ColorRGBA,
  emptyLine,
  emptyTick,
  emptyFill, } from '@arction/lcjs';
import { AppConfiguration } from './../app.configuration';
import { HttpClientDataService } from './../http-client-data.service';
import { WebSocketService, TYPE_START, TYPE_STOP, TYPE_UPDATE } from './../web-socket.service';
import { IntervalPerformanceService } from './../interval-performance.service';
import { DummyService } from './../dummy.service';
import { LcDataService } from './../lc-data.service';
import { LcService } from './../lc.service';

const MINIMUM_Z = 0.0;
const MAXIMUM_Z = 100.0;

const REAL_TIME_MINIMUM_Z = 0.0;
const REAL_TIME_MAXIMUM_Z = 50.0;

const FIRST_X = 0.0;
const FIRST_Y = 0.0;

const LAST_X = 600.0;
const LAST_Y = 400.0;

const RES_X = 600.0;
const RES_Y = 400.0;

const SIZE_WIDTH = 680.0;
const SIZE_HEIGHT = 440.0;

@Component({
  selector: 'app-lc',
  templateUrl: './lc.component.html',
  styleUrls: ['./lc.component.css']
})
export class LcComponent implements OnDestroy, AfterViewInit {
  @ViewChild("result") result: ElementRef;

  error: string;

  dummy: any[];
  data: Matrix<number>;

  chart: ChartXY;
  axisX: Axis;
  chartOptions: EngineOptions & ChartXYOptions;
  heatMap: IntensityGridSeries;
  startPoint: Point;
  endPoint: Point;

  xAtFrom: number = 0.0;
  xAtTo: number = 0.0;

  timer: Subscription;

  private createDummy(resX: number, resY: number): void {
    this.dummy = this.dummyService.dataGenerator(resX, resY, MINIMUM_Z, MAXIMUM_Z);
  }

  /* See Audio Visualizer Showcase */
  private hideAxis(axis: Axis, start: number, end: number): void {
    axis
    .setNibStyle(emptyLine)
    .setTickStrategy(AxisTickStrategies.Empty)
    .setStrokeStyle(emptyLine)
    .setTitleMargin(1)
    .setTitleFillStyle(emptyFill)
    .setInterval(start, end);
  }
  
  constructor(
    private performanceService: IntervalPerformanceService,
    private lcService: LcService,
    private dummyService: DummyService,
    private dataService: HttpClientDataService,
    private webSocketService: WebSocketService) {
    console.log("Construct the LC Component");
    this.startPoint = { x: FIRST_X, y: FIRST_Y };
    this.endPoint = {x: LAST_X, y: LAST_Y };
    this.webSocketService.connect();
  }

  ngAfterViewInit() {
    const lcConfig: LcConfiguration = AppConfiguration.settings.lc;
    const fill: PalettedFill = this.lcService.getFill(lcConfig.interpolate);

    this.chartOptions = this.lcService.createChartOptions(SIZE_HEIGHT, SIZE_WIDTH);
    this.chart = lightningChart().ChartXY(this.chartOptions);

    // Set chart title
    this.chart.setTitle('Experiment Project A');

    this.createDummy(RES_X, RES_Y);

    this.heatMap = this.chart.addHeatmapSeries({
      rows: RES_Y,
      columns: RES_X,
      pixelate: lcConfig.pixelate,
      start: this.startPoint,
      end: this.endPoint
    });
    this.heatMap.invalidateValuesOnly(this.dummy);
    this.heatMap.setFillStyle(fill);

    this.xAtFrom = 0.0;
    this.xAtTo = RES_X;

    /* Hide the default axis */
    this.hideAxis(this.chart.getDefaultAxisX(), this.xAtFrom, this.xAtTo);
    this.axisX = this.chart.addAxisX();
    this.axisX.setInterval(this.xAtFrom, this.xAtTo, false, false);

    /* Disable animation */
    this.chart.getDefaultAxisY().setAnimationScroll(false);

    this.webSocketService.dataUpdates().subscribe( (message: WsMessage) => {
      if(message.t === TYPE_UPDATE && message.u) {
        if(this.performanceService.isStarted) {
          let duration = this.performanceService.completed();
          this.performanceService.update(duration);
          this.performanceService.calculate();
  
          let resultElement = <HTMLDivElement>this.result.nativeElement;
          resultElement.innerHTML = "Interval - " + this.performanceService.format();
        }
        this.performanceService.start();

        let update: WsUpdate = message.u;
        let line = LcDataService.lineGenerator(RES_Y, update.v);
        this.xAtFrom++;
        this.xAtTo++;
        this.axisX.setInterval(this.xAtFrom, this.xAtTo, false, false);
        this.heatMap.addColumn(1, 'value', [line]);
      }
    });
  }

  start(): void {
    const lcConfig: LcConfiguration = AppConfiguration.settings.lc;

    console.log("Show Matrix Button Clicked");

    this.dataService.getMatrix(RES_X).subscribe(m => {
      console.log("Got Matrix");

      this.data = LcDataService.dataGenerator(RES_X, RES_Y, m);

      let ranges: DataRanges = m.r;

      const ratioStep: number = 1.0 / 6.0;
      let ratio = 0.0;

      let steps: LUTStep[] = [];
      steps.push(LcService.createStep(ranges.y, ratio, ColorRGBA(0x07, 0x04, 0x9b))); /* Dark blue */
      ratio += ratioStep;
      steps.push(LcService.createStep(ranges.y, ratio, ColorRGBA(0x02, 0xf7, 0xf3))); /* Cyan      */
      ratio += ratioStep;
      steps.push(LcService.createStep(ranges.y, ratio, ColorRGBA(0x09, 0xf7, 0x25))); /* Green     */
      ratio += ratioStep;
      steps.push(LcService.createStep(ranges.y, ratio, ColorRGBA(0xf4, 0xec, 0x04))); /* Yellow    */
      ratio += ratioStep;
      steps.push(LcService.createStep(ranges.y, ratio, ColorRGBA(0xf7, 0x9d, 0x01))); /* Orange    */
      ratio += ratioStep;
      steps.push(LcService.createStep(ranges.y, ratio, ColorRGBA(0x8c, 0x01, 0x01))); /* Dark red  */
      
      let fill: PalettedFill = new PalettedFill( { lut: new LUT( {
        steps: steps, interpolate: lcConfig.interpolate
        } ) } );

      this.heatMap.invalidateValuesOnly(this.data);
      this.heatMap.setFillStyle(fill);

      this.performanceService.initialize(true);

      let wsStart: WsStart = { f: RES_X };
      let wsMessage: WsMessage = { t: TYPE_START, s: wsStart };
      this.webSocketService.sendMessage(wsMessage);
    });
  }

  stop(): void {
    console.log("Stop Button Clicked");

    let wsMessage: WsMessage = { t: TYPE_STOP };
    this.webSocketService.sendMessage(wsMessage);
  }

  ngOnDestroy(): void {
    this.webSocketService.close();
    // "dispose" should be called when the component is unmounted to free all the resources used by the chart
    this.chart.dispose();
  }
}
