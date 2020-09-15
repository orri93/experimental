import { Component, OnInit } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import * as Highcharts from 'highcharts';

import darkUnicaTheme from 'highcharts/themes/dark-unica'; 
import Boost from 'highcharts/modules/boost';
import noData from 'highcharts/modules/no-data-to-display';
import More from 'highcharts/highcharts-more';

// Changing themes using highcharts-angular
// https://stackoverflow.com/questions/52958769/changing-themes-using-highcharts-angular
// https://github.com/highcharts/highcharts/tree/master/js/masters/themes
darkUnicaTheme(Highcharts);

// Highcharts and Angular 7
// https://www.highcharts.com/blog/tutorials/highcharts-and-angular-7
// https://github.com/m-haziq/highcharts-simplified
Boost(Highcharts);
noData(Highcharts);
More(Highcharts);
noData(Highcharts);

@Component({
  selector: 'app-simple-chart',
  templateUrl: './simple-chart.component.html',
  styleUrls: ['./simple-chart.component.css']
})
export class SimpleChartComponent implements OnInit {

  Highcharts: typeof Highcharts = Highcharts;
  chartOptions: Highcharts.Options = {
    title: {
      text: 'Sample Scatter Plot'
    },
    series: [
      {
        name: "s1",
        data: [[1, 1], [1, 2], [3, 3]],
        type: 'scatter'
      },
      {
        name: "s2",
        data: [[0, 0], [5, 4], [7, 8]],
        type: 'scatter'
      }
    ],
  }

  constructor(private http: HttpClient) {}

  ngOnInit(): void {
    // Sample API
    const apiLink = '/assets/data.json';

    this.getApiResponse(apiLink).then(
      result => {
        console.log("Getting Data from API " + apiLink + ": " + result);
        this.chartOptions.series[0]['data'] = result;
      },
      reason => {
        console.log("Error getting API response for " + apiLink + ": " + reason);
      });
  }

  getApiResponse(url) {
    return this.http.get(url, {}).toPromise().then(res => {
      return res;
    });
  }

}
