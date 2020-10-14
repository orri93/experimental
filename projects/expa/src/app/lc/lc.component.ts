import { Component, AfterViewInit, OnDestroy } from '@angular/core';
import { interval, Subscription } from 'rxjs';
import {
  Axis,
  Point,
  Matrix,
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
  emptyLine,
  emptyTick,
  emptyFill, } from '@arction/lcjs';
import { AppConfiguration } from './../app.configuration';
import { LcService } from './../lc.service';
import { DummyService } from './../dummy.service';


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

const INTERPOLATED_PALLET = true;
const PIXELATE = false;

@Component({
  selector: 'app-lc',
  templateUrl: './lc.component.html',
  styleUrls: ['./lc.component.css']
})
export class LcComponent implements OnDestroy, AfterViewInit {
  
  error: string;

  dummy: any[];
  data: Matrix<number>;

  chart: ChartXY;
  axisX: Axis;
  chartOptions: EngineOptions & ChartXYOptions;
  heatMap: IntensityGridSeries;
  start: Point;
  end: Point;

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
  
  constructor(private lcService: LcService, private dummyService: DummyService) {
    console.log("Construct the LC Component");
    this.start = { x: FIRST_X, y: FIRST_Y };
    this.end = {x: LAST_X, y: LAST_Y };
  }

  ngAfterViewInit() {
    const fill: PalettedFill = this.lcService.getFill(INTERPOLATED_PALLET);

    this.chartOptions = this.lcService.createChartOptions(SIZE_HEIGHT, SIZE_WIDTH);
    this.chart = lightningChart().ChartXY(this.chartOptions);

    // Set chart title
    this.chart.setTitle('Experiment Project A');

    this.createDummy(RES_X, RES_Y);

    this.heatMap = this.chart.addHeatmapSeries({
      rows: RES_Y,
      columns: RES_X,
      pixelate: PIXELATE,
      start: this.start,
      end: this.end
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
  }

  ngOnDestroy(): void {
    // "dispose" should be called when the component is unmounted to free all the resources used by the chart
    this.chart.dispose();
  }
}
