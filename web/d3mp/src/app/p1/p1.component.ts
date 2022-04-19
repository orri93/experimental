import { Component, ViewChild, AfterViewInit, OnInit, OnDestroy } from '@angular/core';
import { Subscription, Observable, interval } from 'rxjs';

import { M1c1Component } from './../m1/m1c1/m1c1.component';

@Component({
  selector: 'app-p1',
  templateUrl: './p1.component.html',
  styleUrls: ['./p1.component.scss']
})
export class P1Component implements OnInit, AfterViewInit, OnDestroy {
  @ViewChild('m1c1') m1c1!: M1c1Component;

  pageName = 'Page no. 1';

  private observable: Observable<number>;
  private timer!: Subscription;
  private t = 1.0;

  constructor() {
    this.observable = interval(100);
  }

  private f(x: number, a: number): number {
    return x * x / a - 1.0;
  }

  private createGraph(a: number, range: NumberRange, count: number): Point[] {
    const graph: Point[] = [];
    for(let x = range.from; x <= range.to; x += (range.to - range.from) / count) {
      graph.push({x, y: this.f(x, a)});
    }
    return graph;
  }

  private onTimer(): void {
    console.log('On Timer');
    this.m1c1.clear();
    const range: NumberRange = { from: -2.0, to: 2.0 };
    const points: Point[] = this.createGraph(this.t, range, 100.0);
    this.t += 0.01;
    if (this.t > 2.0) {
      this.t = 1.0;
    }
    this.m1c1.draw(points);
  }

  private createTimer(): void {
    this.t = 1.0;
    this.timer = this.observable.subscribe(( x => { this.onTimer(); }));
  }

  private start(): void {
    if (this.timer) {
      if (this.timer.closed) {
        this.createTimer();
      } else {
        console.warn('Timer is still opened');
      }
    } else {
      this.createTimer();
    }
  }

  private stop(): void {
    if (this.timer) {
      if (this.timer.closed) {
        console.warn('Timer is already closed');
      } else {
        this.timer.unsubscribe();
      }
    } else {
      console.warn('The timer is still undefined');
    }
  }

  private reset(): void {
    console.log('Reset');
    this.m1c1.clear();
  }

  ngOnInit(): void {
  }

  ngAfterViewInit(): void {
    const range: NumberRange = { from: -2.0, to: 2.0 };
    const points: Point[] = this.createGraph(1.0, range, 100.0);
    this.m1c1.draw(points);
  }

  ngOnDestroy(): void {
    if (this.timer && !this.timer.closed) {
      console.log('Destroy and unsubscribe');
      this.timer.unsubscribe();
    } else {
      console.log('Destroy');
    }
  }

  onStart(): void {
    console.log('On Start');
    if (this.m1c1) {
      this.start();
    }
  }

  onStop(): void {
    console.log('On Stop');
    if (this.m1c1) {
      this.stop();
    }
  }

  onReset(): void {
    console.log('On Reset');
    if (this.m1c1) {
      this.reset();
    }
  }
}
