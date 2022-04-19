import { Component, OnInit } from '@angular/core';

import { GraphService } from './../m1/graph.service';

@Component({
  selector: 'app-p2',
  templateUrl: './p2.component.html',
  styleUrls: ['./p2.component.scss']
})
export class P2Component implements OnInit {
  pageName = 'Page no. 2';

  constructor() { }

  private f(x: number, w: number): number {
    return Math.sin(w * x);
  }

  ngOnInit(): void {
    const count = 3000;
    const l = 64;
    const lz = l + 0.5;
    const ticks: GraphTickValues = {
      x: [],
      y: [-1.0, 1.0]
    };
    const range: GraphRange = {
      x: {from: -lz, to: lz},
      y: {from: -1.5, to: 1.5}
    };
    const scale: GraphSize = {
      width: 10,
      height: 40
    };

    const graphRange: NumberRange = {from: -l, to: l};

    const graphService1 = new GraphService();
    const graphService2 = new GraphService();
    const graphService3 = new GraphService();
    const graphService4 = new GraphService();

    graphService1.initialize('graph1', ticks, range, scale);
    graphService1.drawAxes();
    graphService2.initialize('graph2', ticks, range, scale);
    graphService2.drawAxes();
    graphService3.initialize('graph3', ticks, range, scale);
    graphService3.drawAxes();
    range.y.from = -3.0;
    range.y.to = 3.0;
    graphService4.initialize('graph4', ticks, range, scale);
    graphService4.drawAxes();

    const graph1: Point[] = [];
    const graph2: Point[] = [];
    const graph3: Point[] = [];
    const graph4: Point[] = [];
    for(let x = graphRange.from; x <= graphRange.to; x += (graphRange.to - graphRange.from) / count) {
      const f1 = this.f(x, 2.0);
      const f2 = this.f(x, 2.5);
      const f3 = this.f(x, 3.3);
      const f4 = f1 + f2 + f3;
      graph1.push({x, y: f1});
      graph2.push({x, y: f2});
      graph3.push({x, y: f3});
      graph4.push({x, y: f4});
    }

    graphService1.draw(graph1);
    graphService2.draw(graph2);
    graphService3.draw(graph3);
    graphService4.draw(graph4);
  }

}
