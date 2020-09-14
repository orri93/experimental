import * as Highcharts from 'highcharts';
import * as HighchartsData from 'highcharts/modules/data';
import darkUnicaTheme from 'highcharts/themes/dark-unica';
import { Component, OnInit } from '@angular/core';

// Changing themes using highcharts-angular
// https://stackoverflow.com/questions/52958769/changing-themes-using-highcharts-angular
// https://github.com/highcharts/highcharts/tree/master/js/masters/themes
darkUnicaTheme(Highcharts);

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
    HighchartsData.Highcharts.getJSON(
      'https://cdn.jsdelivr.net/gh/highcharts/highcharts@v7.0.0/samples/data/usdeur.json',
      function(jsondata) {
        this.chartOptions.series = [{
          type: 'area',
          name: 'USD to EUR',
          data: jsondata
        }];
      }
    )
  }

}
