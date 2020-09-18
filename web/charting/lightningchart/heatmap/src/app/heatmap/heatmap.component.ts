import { Component, AfterViewInit, OnDestroy } from '@angular/core';
import { lightningChart, IntensityGridSeries, PalettedFill, ChartXY, Themes, Point } from '@arction/lcjs';
import { interval, Subscription } from 'rxjs';
import { HeatmapService } from './../heatmap.service';
import { DataService } from './../data.service';

const MINIMUM_Z = 0.0;
const MAXIMUM_Z = 64.0;

const REAL_TIME_MINIMUM_Z = 0.0;
const REAL_TIME_MAXIMUM_Z = 32.0;

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
  heatmap: IntensityGridSeries;
  chart: ChartXY;
  start: Point;
  end: Point;
  timer: Subscription;
  xat: number = 0.0;

  timerCount = 0.0;
  timerSum = 0.0;

  private createRandom(resx: number, resy: number): void {
    this.data = this.dataService.dataGenerator(resx, resy, MINIMUM_Z, MAXIMUM_Z);
  }

  constructor(private heatmapService: HeatmapService, private dataService: DataService) {
    this.start = { x: FIRST_X, y: FIRST_Y };
    this.end = {x: LAST_X, y: LAST_Y };
  }

  ngAfterViewInit() {
    performance.mark(MARKER_NAME_LOAD_START);

    const fill: PalettedFill = this.heatmapService.getFill(INTERPOLATED_PALLET);

    // Create chartXY
    this.chart = lightningChart().ChartXY(
      {
        container: "lcjsc",
        theme: Themes.dark,
        height: SIZE_HEIGHT,
        width: SIZE_WIDTH
      });

    // Set chart title
    this.chart.setTitle('Random Data');

    this.createRandom(RES_X, RES_Y);

    this.heatmap = this.chart.addHeatmapSeries({
      rows: RES_Y,
      columns: RES_X,
      start: this.start,
      end: this.end,
      pixelate: PIXELATE
    });
    this.heatmap.invalidateValuesOnly(this.data);
    this.heatmap.setFillStyle(fill);
    this.xat = RES_X;

    performance.mark(MARKER_NAME_LOAD_COMPLETED);
    performance.measure(MEASURE_LOAD, MARKER_NAME_LOAD_START, MARKER_NAME_LOAD_COMPLETED);

    var performanceDivElement = document.getElementById("performance-load-result");
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

    let columns = this.data[0].length;
    console.log("Timer column count: " + columns);
    let rows = this.data.length;

    this.heatmap.addColumn(2, 'value', [[10, 20], [50, 68]]);

    performance.mark(MARKER_NAME_TIMER_COMPLETED);
    performance.measure(MEASURE_TIMER, MARKER_NAME_TIMER_START, MARKER_NAME_TIMER_COMPLETED);

    var performanceTimerDivElement = document.getElementById("performance-timer-result");
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
