import { Component, OnInit } from '@angular/core';

import * as d3 from 'd3';

const gamma = 2;
const c = 10;

@Component({
  selector: 'app-p1',
  templateUrl: './p1.component.html',
  styleUrls: ['./p1.component.scss']
})
export class P1Component implements OnInit {

  pageName = 'Page no. 1';

  constructor() { }

  private f(x: number, y: number): [number, number] {
    const rho = Math.sqrt(x * x + y * y);
    const r = rho - 1;
    const a = gamma * (1 - rho * rho);
    const b = -gamma * c * r * r  + 1;
    return [x * a - b * y, y * a + b * x];
  }

  ngOnInit(): void {
  }
}
