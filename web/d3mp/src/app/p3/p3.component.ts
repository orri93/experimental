import { Component, OnInit, OnDestroy } from '@angular/core';
import { Subscription, Observable, interval } from 'rxjs';

import { GraphService } from './../m1/graph.service';

const l = 4;
const count = 1000;
const graphRange: NumberRange = {from: -l, to: l};

@Component({
  selector: 'app-p3',
  templateUrl: './p3.component.html',
  styleUrls: ['./p3.component.scss']
})
export class P3Component implements OnInit, OnDestroy {
  pageName = 'Page no. 3';

  private observable: Observable<number>;
  private timer!: Subscription;
  private t = 0.0;

  private graphService1: GraphService;
  private graphService2: GraphService;
  private graphService3: GraphService;
  private graphService4: GraphService;

  constructor() {
    this.observable = interval(100);
    this.graphService1 = new GraphService();
    this.graphService2 = new GraphService();
    this.graphService3 = new GraphService();
    this.graphService4 = new GraphService();
  }

  private fs(x: number, w: number, t: number): number {
    const a = Math.abs(x);
    if (x >= 0) {
      return Math.exp(-a) * Math.sin(t + w * Math.PI * a);
    } else {
      return -Math.exp(-a) * Math.sin(t + w * Math.PI * a);
    }
  }

  private fc(x: number, w: number, t: number): number {
    const a = Math.abs(x);
    return Math.exp(-a) * Math.cos(t + w * Math.PI * a);
  }

  private onTimer(): void {
    console.log('On Timer');
    const graph1: Point[] = [];
    const graph2: Point[] = [];
    const graph3: Point[] = [];
    const graph4: Point[] = [];
    for(let x = graphRange.from; x <= graphRange.to; x += (graphRange.to - graphRange.from) / count) {
      const f1 = this.fc(x, 2.0, this.t);
      const f2 = this.fs(x, 2.5, this.t);
      const f3 = this.fc(x, 3.3, this.t);
      const f4 = f1 + f2 + f3;
      graph1.push({x, y: f1});
      graph2.push({x, y: f2});
      graph3.push({x, y: f3});
      graph4.push({x, y: f4});
    }

    this.graphService1.clear();
    this.graphService2.clear();
    this.graphService3.clear();
    this.graphService4.clear();

    this.graphService1.draw(graph1);
    this.graphService2.draw(graph2);
    this.graphService3.draw(graph3);
    this.graphService4.draw(graph4);

    this.t += 0.1;
    if (this.t > Math.PI) {
      this.t = 0.0;
    }
  }

  private createTimer(): void {
    this.t = 0.0;
    this.timer = this.observable.subscribe(( x => { this.onTimer(); }));
  }

  ngOnInit(): void {
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
      width: 100,
      height: 40
    };

    this.graphService1.initialize('graph1', ticks, range, scale);
    this.graphService1.drawAxes();
    this.graphService2.initialize('graph2', ticks, range, scale);
    this.graphService2.drawAxes();
    this.graphService3.initialize('graph3', ticks, range, scale);
    this.graphService3.drawAxes();
    range.y.from = -3.0;
    range.y.to = 3.0;
    this.graphService4.initialize('graph4', ticks, range, scale);
    this.graphService4.drawAxes();

    this.createTimer();
  }

  ngOnDestroy(): void {
    if (this.timer && !this.timer.closed) {
      console.log('Destroy and unsubscribe');
      this.timer.unsubscribe();
    } else {
      console.log('Destroy');
    }
  }
}
