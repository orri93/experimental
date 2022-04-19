import { Component, ViewChild, AfterViewInit, OnInit } from '@angular/core';

import { M1c1Component } from './../m1/m1c1/m1c1.component';

@Component({
  selector: 'app-p0',
  templateUrl: './p0.component.html',
  styleUrls: ['./p0.component.scss']
})
export class P0Component implements OnInit, AfterViewInit {
  @ViewChild('m1c1') m1c1!: M1c1Component;

  pageName = 'Page no. 0';

  constructor() { }

  private f(t: number): number {
    return Math.sin(t);
  }

  private createGraph(range: NumberRange, count: number): Point[] {
    const graph: Point[] = [];
    for(let x = range.from; x <= range.to; x += (range.to - range.from) / count) {
      graph.push({x, y: this.f(x)});
    }
    return graph;
  }

  ngOnInit(): void {
  }

  ngAfterViewInit(): void {
    const range: NumberRange = { from: -4.0, to: 4.0 };
    const points: Point[] = this.createGraph(range, 200.0);
    this.m1c1.draw(points);
  }
}
