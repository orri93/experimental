import { Component, OnInit, AfterViewInit } from '@angular/core';

import { SciChartSurface } from 'scichart';
import { NumericAxis } from "scichart/Charting/Visuals/Axis/NumericAxis";

const DIV_SCICHART_HEATMAP_ELEMENT_ID = "scichart-heatmap";

@Component({
  selector: 'app-heatmap',
  templateUrl: './heatmap.component.html',
  styleUrls: ['./heatmap.component.css']
})
export class HeatmapComponent implements OnInit, AfterViewInit {

  constructor() { }

  ngOnInit(): void {
  }

  ngAfterViewInit(): void {
    SciChartSurface.create(DIV_SCICHART_HEATMAP_ELEMENT_ID).then( (value) => {

    });
  }

  public start(): void {
    console.log("Start button clicked");
  }

  public stop(): void {
    console.log("Start button clicked");
  }
}
