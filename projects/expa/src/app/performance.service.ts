import { Injectable } from '@angular/core';

const MARKER_NAME_START = "performance-start";
const MARKER_NAME_COMPLETED = "performance-completed";
const MEASURE = "performance-measure";

const FORMAT_PRECISION = 6;

@Injectable({ providedIn: 'root' })
export class PerformanceService {

  items: number[] = [];
  sum = 0.0;
  count = 0.0;
  minimum = 0.0;
  maximum = 0.0;
  average = 0.0;
  last = 0.0;
  sd = 0.0;

  withSd = false;

  private calcSd(average: number): number {
    let sdSum = 0.0;
    this.items.forEach(element => {
      let diff = element - average;
      sdSum += diff * diff;
    });
    return Math.sqrt(sdSum / this.count);
  }

  public initialize(withSd: boolean): void {
    this.withSd = withSd;
    this.items = [];
    this.sum = 0.0;
    this.count = 0.0;
    this.minimum = Number.MAX_VALUE;
    this.maximum = -Number.MAX_VALUE;
  }
  
  public update(value: number): void {
    if(this.withSd) {
      this.items.push(value);
    }
    this.count++;
    this.sum += value;
    this.minimum = Math.min(this.minimum, value);
    this.maximum = Math.max(this.maximum, value);
    this.last = value;
  }

  public calculate(): void {
    this.average = this.sum / this.count;
    if(this.withSd) {
      this.sd = this.calcSd(this.average);
    }
  }

  public format(): string {
    let min = this.minimum.toFixed(FORMAT_PRECISION);
    let max = this.maximum.toFixed(FORMAT_PRECISION);
    let count = this.count.toFixed(FORMAT_PRECISION);
    let fa = this.average.toFixed(FORMAT_PRECISION);
    let l = this.last.toFixed(FORMAT_PRECISION);
    if(this.withSd) {
      let fsd = this.sd.toFixed(FORMAT_PRECISION);
      return `Average: ${fa}, SD: ${fsd}, Minimum: ${min}, Maximum: ${max}, Last: ${l}, Count: ${count}`;
    } else {
      return `Average: ${fa}, Minimum: ${min}, Maximum: ${max}, Last: ${l}, Count: ${count}`;
    }
  }

  public start(): void {
    performance.mark(MARKER_NAME_START);
  }

  public completed(): number {
    performance.clearMeasures();
    
    performance.mark(MARKER_NAME_COMPLETED);
    performance.measure(MEASURE, MARKER_NAME_START, MARKER_NAME_COMPLETED);

    let entries = performance.getEntriesByType("measure");
    let firstEntry = entries[0];
    let duration = firstEntry.duration;

    return duration;
  }

  public clear(): void {
    performance.clearMarks();
    performance.clearMeasures();
  }
}
