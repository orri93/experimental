import { Component, OnInit } from '@angular/core';
import * as Highcharts from 'highcharts';

import darkUnicaTheme from 'highcharts/themes/dark-unica'; 
import Heatmap from 'highcharts/modules/heatmap';

darkUnicaTheme(Highcharts);
Heatmap(Highcharts);

const NUMBER_OF_POINTS: number = 1000;

let heatmapComponent: HeatmapComponent;

@Component({
  selector: 'app-heatmap',
  templateUrl: './heatmap.component.html',
  styleUrls: ['./heatmap.component.css']
})
export class HeatmapComponent implements OnInit {

  public data: any[] = [];
  public Highcharts: typeof Highcharts = Highcharts;

  public chartOptions: Highcharts.Options = {
    chart: {
      zoomType: 'x',
    },
    boost: {
      useGPUTranslations: true,
      usePreallocated: true
    },

    title: {
      text: 'Random heatmap'
    },

    colorAxis: {
      min:0,
      minColor: '#011F4B',
      maxColor: '#D3E2ED'
    },

    legend: {
      align: 'right',
      layout: 'vertical',
      margin: 0,
      verticalAlign: 'top',
      y: 25,
      symbolHeight: 280
    },

    series: [
      {
        name: '1 million heatmap points',
        borderWidth: 1,
        type: 'heatmap',
        data: []
      }
    ]
  }

  constructor() {
    heatmapComponent = this;
  }

  ngOnInit(): void {}

  getRandomInt(min: number, max: number): number {
    min = Math.ceil(min);
    max = Math.floor(max);
    return Math.floor(Math.random() * (max - min)) + min; //The maximum is exclusive and the minimum is inclusive
  }

  initData(pointsAmount: number) {
    this.data = [];
    for(let i: number = 0; i < pointsAmount; i++) {
      for(let j: number = 0; j < pointsAmount; i++) {
        this.data.push([i, j, this.getRandomInt(0, 3)]);
      }
    }
  }

  adjustData(points: any[], plotWidth: number, plotHeight: number): any[] {
    let availablePoints: number = plotWidth * plotHeight;
    let pointsAmount: number = points.length;
    let pointsCounter: number = Math.floor(points.length / availablePoints);
    this.data = [];

    if(pointsAmount > availablePoints) {
      for(let i: number = 0; i < pointsAmount; i += pointsCounter) {
        this.data.push(points[i]);
      }
      return this.data;
    } else {
      return points;
    }
  }

  chartCallback(ch: Highcharts.Chart) {

    console.log("Chart callback Plot width and height: " + ch.plotWidth + " , " + ch.plotHeight);

    heatmapComponent.initData(NUMBER_OF_POINTS);
    let adjustedData: any[] = this.adjustData(
      this.data,
      ch.plotWidth,
      ch.plotHeight);
    
    ch.series[0].data = adjustedData;
    
  }

}
