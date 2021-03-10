import { Injectable } from '@angular/core';
import * as moment from 'moment';
import { Point } from './../models/wd3.model';

@Injectable({
  providedIn: 'root'
})
export class MockDataService {

  private points: Point[];

  constructor() { }

  static evolvePoint(point: Point, valueFactor = 0.1, depthFactor = 0.1, randomDepth = false): void {
    point.depth += randomDepth ? Math.random() * depthFactor : depthFactor;
    point.value += Math.random() * valueFactor;
  }

  evolve(index: number): Point {
    const point: Point = { depth: this.points[index].depth, value: this.points[index].value };
    MockDataService.evolvePoint(this.points[index]);
    return point;
  }

  initialize(toolCount: number): void {
    let value = 5.0 + Math.random();
    this.points = [];
    for (let i = 0; i < toolCount; i++) {
      const n = i + 1;
      const point: Point = { depth: 100.0 * n, value };
      this.points.push(point);
      value += Math.random() + 10.0 * n;
    }
  }
}
