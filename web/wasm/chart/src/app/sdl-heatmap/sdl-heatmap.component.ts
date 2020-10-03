import { Component, ViewChild, ElementRef, NgZone, OnDestroy, AfterViewInit } from '@angular/core';
import { interval, Subscription } from 'rxjs';
import { HttpClient } from "@angular/common/http";
import { EmscriptenWasmComponent } from "../emscripten-wasm.component";

const TIMER_INTERVAL = 1.0;

const MARKER_NAME_LOAD_START = "load-start";
const MARKER_NAME_LOAD_COMPLETED = "load-completed";
const MEASURE_LOAD = "load-measure";

const MARKER_NAME_TIMER_START = "timer-start";
const MARKER_NAME_TIMER_COMPLETED = "timer-completed";
const MEASURE_TIMER = "timer-measure";

const requestFullscreen =
  document.documentElement.requestFullscreen ||
  document.documentElement["webkitRequestFullscreen"] ||
  document.documentElement["msRequestFullscreen"] ||
  document.documentElement["mozRequestFullScreen"];

@Component({
  selector: 'app-sdl-heatmap',
  templateUrl: './sdl-heatmap.component.html',
  styleUrls: ['./sdl-heatmap.component.css']
})
export class SdlHeatmapComponent extends EmscriptenWasmComponent implements OnDestroy {
  @ViewChild("canvas") canvas: ElementRef;
  error: string;
  supportsFullscreen: boolean;
  timer: Subscription;
  loadDuration = 0.0;
  timerCount = 0.0;
  timerSum = 0.0;

  constructor(private httpClient: HttpClient, private ngZone: NgZone) {
    super("ChartModule", "goschart.js");

    performance.mark(MARKER_NAME_LOAD_START);

    this.supportsFullscreen = !!requestFullscreen;

    this.moduleDecorator = (mod) => {
      mod.arguments = [];
      mod.preRun = [];
      mod.canvas = <HTMLCanvasElement>this.canvas.nativeElement;
      mod.printErr = (what: string) => {
        if (!what.startsWith("WARNING")) {
          this.ngZone.run(() => (this.error = what));
        }
      };
    };

    performance.mark(MARKER_NAME_LOAD_COMPLETED);
    performance.measure(MEASURE_LOAD, MARKER_NAME_LOAD_START, MARKER_NAME_LOAD_COMPLETED);

    var entries = performance.getEntriesByType("measure");
    var firstentry = entries[0];

    this.loadDuration = firstentry.duration;

    performance.clearMarks();
    performance.clearMeasures();
  }

  toggleFullscreen() {
    if (requestFullscreen) {
      requestFullscreen.bind(this.canvas.nativeElement)();
    }
  }

  step() {
    if(this.module) {
      this.module.ccall("step", "void", [], []);
    }
  }

  start() {
    console.log("Start button clicked");
    if(this.timer == null || this.timer.closed) {
      this.timer = interval(TIMER_INTERVAL).subscribe(( x => {
        this.onTimer();
      }));
      console.log("Starting timer subscription.");
    } else {
      console.log("Timer subscription already started.");
    }
  }

  stop() {
    console.log("Stop button clicked");
    if(this.timer.closed) {
      console.log("Timer already closed.");
    } else {
      console.log("Unsubscribe the timer.");
      this.timer.unsubscribe();
    }
  }

  onTimer(): void {
    performance.mark(MARKER_NAME_TIMER_START);

    if(this.module) {
      this.module.ccall("line", "void", [], []);
    }
    
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

  //ngAfterViewInit() {
  //  var performanceDivElement = document.getElementById("performance-load-result-id");
  //  performanceDivElement.innerHTML = "Load time: " + this.loadDuration + " ms";
  //}
  
  ngOnDestroy(): void {
    console.log("On ng destroy");
    if(this.module) {
      console.log("Calling shutdown in module");
      this.module.ccall("shutdown", "void", [], []);
    }
  }

}
