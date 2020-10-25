import { Injectable } from '@angular/core';
import { PerformanceService } from './performance.service';

const INTERVAL_MARKER_NAME_START = "performance-start";
const INTERVAL_MARKER_NAME_COMPLETED = "performance-completed";
const INTERVAL_MEASURE = "performance-measure";

@Injectable({ providedIn: 'root' })
export class IntervalPerformanceService extends PerformanceService {
  isStarted = false;

  public initialize(withSd: boolean): void {
    super.initialize(withSd);
    this.isStarted = false;
  }

  public start(): void {
    performance.mark(INTERVAL_MARKER_NAME_START);
    this.isStarted = true;
  }

  public completed(): number {
    performance.clearMeasures();
    
    performance.mark(INTERVAL_MARKER_NAME_COMPLETED);
    performance.measure(
      INTERVAL_MEASURE,
      INTERVAL_MARKER_NAME_START,
      INTERVAL_MARKER_NAME_COMPLETED);

    let entries = performance.getEntriesByType("measure");
    let firstEntry = entries[0];
    let duration = firstEntry.duration;

    return duration;
  }
}
