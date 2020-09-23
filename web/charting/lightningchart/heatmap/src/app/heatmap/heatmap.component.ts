import { Component, AfterViewInit, OnDestroy } from '@angular/core';
import {
  Axis,
  Point,
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
import { interval, Subscription } from 'rxjs';
import { HeatmapService } from './../heatmap.service';
import { DataService } from './../data.service';

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

const SIZE_WIDTH = 600.0;
const SIZE_HEIGHT = 400.0;

const TIMER_INTERVAL = 100.0;

const INTERPOLATED_PALLET = true;
const PIXELATE = false;

const MARKER_NAME_LOAD_START = "load-start";
const MARKER_NAME_LOAD_COMPLETED = "load-completed";
const MEASURE_LOAD = "load-measure";

const MARKER_NAME_TIMER_START = "timer-start";
const MARKER_NAME_TIMER_COMPLETED = "timer-completed";
const MEASURE_TIMER = "timer-measure";

@Component({
  selector: 'app-heatmap',
  templateUrl: './heatmap.component.html',
  styleUrls: ['./heatmap.component.css']
})
export class HeatmapComponent implements OnDestroy, AfterViewInit {

  data: any[];
  chart: ChartXY;
  axisx: Axis;
  chartOptions: EngineOptions & ChartXYOptions;
  heatmap: IntensityGridSeries;
  start: Point;
  end: Point;

  xatfrom: number = 0.0;
  xatto: number = 0.0;

  timer: Subscription;

  timerCount = 0.0;
  timerSum = 0.0;

  private createRandom(resx: number, resy: number): void {
    this.data = this.dataService.dataGenerator(resx, resy, MINIMUM_Z, MAXIMUM_Z);
  }

  /* See Audio Visualizer Showcase */
  private hideAxis(axis: Axis, start: number, end: number): void {
    axis
      .setNibStyle(emptyLine)
      .setTickStrategy(AxisTickStrategies.Empty)
      .setStrokeStyle(emptyLine)
      .setTitleMargin(1)
      .setTitleFillStyle(emptyFill)
      .setInterval(this.xatfrom, this.xatto);
  }

  constructor(private heatmapService: HeatmapService, private dataService: DataService) {
    this.start = { x: FIRST_X, y: FIRST_Y };
    this.end = {x: LAST_X, y: LAST_Y };
  }

  ngAfterViewInit() {
    performance.mark(MARKER_NAME_LOAD_START);

    const fill: PalettedFill = this.heatmapService.getFill(INTERPOLATED_PALLET);

    this.chartOptions = this.heatmapService.createChartOptions(SIZE_HEIGHT, SIZE_WIDTH);
    this.chart = lightningChart().ChartXY(this.chartOptions);
    
    // Set chart title
    this.chart.setTitle('Random Data');

    this.createRandom(RES_X, RES_Y);

    this.heatmap = this.chart.addHeatmapSeries({
      rows: RES_Y,
      columns: RES_X,
      pixelate: PIXELATE,
      start: this.start,
      end: this.end
    });
    this.heatmap.invalidateValuesOnly(this.data);
    this.heatmap.setFillStyle(fill);

    this.xatfrom = 0.0;
    this.xatto = RES_X;

    /* Hide the default axis */
    this.hideAxis(this.chart.getDefaultAxisX(), this.xatfrom, this.xatto);
    this.axisx = this.chart.addAxisX();
    this.axisx.setInterval(this.xatfrom, this.xatto, false, false);

    /* Disable animtions */
    this.chart.getDefaultAxisY().setAnimationScroll(false);

    performance.mark(MARKER_NAME_LOAD_COMPLETED);
    performance.measure(MEASURE_LOAD, MARKER_NAME_LOAD_START, MARKER_NAME_LOAD_COMPLETED);

    var performanceDivElement = document.getElementById("performance-load-result-id");
    var entries = performance.getEntriesByType("measure");
    var firstentry = entries[0];

    performanceDivElement.innerHTML = "Load time: " + firstentry.duration + " ms";

    performance.clearMarks();
    performance.clearMeasures();
  }

  startRealTime(): void {
    console.log("Start Real Time button clicked.");
    if(this.timer == null || this.timer.closed) {
      this.timer = interval(TIMER_INTERVAL).subscribe(( x => {
        this.onTimer();
      }));
      console.log("Starting timer subscription.");
    } else {
      console.log("Timer subscription already started.");
    }
  }

  stopRealTime(): void {
    console.log("Stop Real Time button clicked.");
    if(this.timer.closed) {
      console.log("Timer already closed.");
    } else {
      console.log("Unsubscribe the timer.");
      this.timer.unsubscribe();
    }
  }

  onTimer(): void {
    performance.mark(MARKER_NAME_TIMER_START);

    let dataline = this.dataService.createLine(RES_Y, REAL_TIME_MINIMUM_Z, REAL_TIME_MAXIMUM_Z);
    this.xatfrom++;
    this.xatto++;
    this.axisx.setInterval(this.xatfrom, this.xatto, false, false);    
    this.heatmap.addColumn(1, 'value', [dataline]);
    
    performance.mark(MARKER_NAME_TIMER_COMPLETED);
    performance.measure(MEASURE_TIMER, MARKER_NAME_TIMER_START, MARKER_NAME_TIMER_COMPLETED);

    var performanceTimerDivElement = document.getElementById("performance-timer-result-id");
    var entriesTimer = performance.getEntriesByType("measure");
    var firstTimerEntry = entriesTimer[0];

    var timerDuration = firstTimerEntry.duration;
    this.timerCount++;
    this.timerSum += timerDuration;
    var average = this.timerSum / this.timerCount;

    performanceTimerDivElement.innerHTML = "Timer. Last: " + timerDuration + " ms, Average: " + average + " ms, Count: " + this.timerCount;

    performance.clearMarks();
    performance.clearMeasures();
  }

  ngOnDestroy() {
    // "dispose" should be called when the component is unmounted to free all the resources used by the chart
    this.chart.dispose();
  }
}
