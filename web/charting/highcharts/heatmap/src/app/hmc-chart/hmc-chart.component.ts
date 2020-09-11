import * as _ from 'lodash';

import { Component, OnInit, OnDestroy, OnChanges, SimpleChanges } from '@angular/core';

@Component({
  selector: 'app-hmc-chart',
  templateUrl: './hmc-chart.component.html',
  styleUrls: ['./hmc-chart.component.css']
})
export class HmcChartComponent implements OnInit, OnDestroy, OnChanges {

  constructor() { }
  
  ngOnInit(): void {
  }

  ngOnChanges(changes: SimpleChanges): void {
  }

  ngOnDestroy(): void {
  }

}
