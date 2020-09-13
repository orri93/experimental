import * as _ from 'lodash';
import * as moment from 'moment';

import { Component, OnInit, OnDestroy, ViewChild, OnChanges, SimpleChanges } from '@angular/core';
import { Subscription, interval, Subject, AsyncSubject, BehaviorSubject } from "rxjs";
import { takeUntil } from 'rxjs/operators';

@Component({
  selector: 'app-hmc-chart',
  templateUrl: './hmc-chart.component.html',
  styleUrls: ['./hmc-chart.component.css']
})
export class HmcChartComponent implements OnInit, OnDestroy, OnChanges {
  @ViewChild('container') container: any;

  private subscriptions: Subscription[] = [];

  constructor() { }

  ngOnInit(): void {

  }

  ngOnChanges(changes: SimpleChanges): void {
  }

  ngOnDestroy(): void {
  }

}
