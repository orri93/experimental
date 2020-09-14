import * as Highcharts from 'highcharts';
import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-simple-chart',
  templateUrl: './simple-chart.component.html',
  styleUrls: ['./simple-chart.component.css']
})
export class SimpleChartComponent implements OnInit {

  Highcharts: typeof Highcharts = Highcharts; // required
  chartOptions: Highcharts.Options = {
    series: [{
      data: [1, 2, 3, 4],
      type: 'line'
    }],
    boost: {
      enabled: true
    }
  }; // required
  // optional function, defaults to null chartCallback: Highcharts.ChartCallbackFunction = function (chart) { } 
  updateFlag: boolean = false; // optional boolean
  oneToOneFlag: boolean = true; // optional boolean, defaults to false
  runOutsideAngular: boolean = false; // optional boolean, defaults to false

  constructor() { }

  ngOnInit(): void {
  }

}
