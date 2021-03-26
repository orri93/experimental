import { Injectable } from '@angular/core';
import { Point } from './../models/m5.model';

@Injectable({
  providedIn: 'root'
})
export class MockService {
  valueStart: number;
  valueInterval: number;
  valueStep: number;
  valueNoise: number;

  depthStart: number;
  depthInterval: number;
  depthStep: number;
  depthNoise: number;

  private count: number;
  private points: Point[];

  constructor() {
    this.reset();
  }

  private static noise(): number {
    return 1.0 - 2.0 * Math.random();
  }

  private evolvePoint(point: Point): void {
    point.value += this.valueStep + this.valueNoise * MockService.noise();
    point.depth += this.depthStep + this.depthNoise * MockService.noise();
  }

  evolve(index: number): Point {
    const point: Point = {
      depth: this.points[index].depth,
      value: this.points[index].value
    };
    this.evolvePoint(this.points[index]);
    return point;
  }

  next(): Point[] {
    const result: Point[] = [];
    for (let i = 0; i < this.count; i++) {
      result.push(this.evolve(i));
    }
    return result;
  }

  reset(): void {
    this.valueStart = 5.0;
    this.depthStart = 100.0;

    this.valueInterval = 10.0;
    this.depthInterval = 100.0;

    this.valueStep = 0.2;
    this.depthStep = 1.0;

    this.valueNoise = 0.1;
    this.depthNoise = 1.0;
  }

  initialize(count: number): void {
    this.count = count;
    let value = this.valueStart;
    let depth = this.depthStart;
    this.points = [];
    for (let i = 0; i < this.count; i++) {
      const point: Point = { depth, value };
      this.points.push(point);
      value += this.valueInterval;
      depth += this.depthInterval;
    }
  }
}
