import { Component, AfterViewInit, OnChanges, OnDestroy } from '@angular/core';
import {
  lightningChart,
  EngineOptions,
  PalettedFill,
  ColorRGBA,
  ChartXY,
  Themes,
  Point,
  LUT } from '@arction/lcjs';

import { DataService } from './../data.service';
import { WaterDropGeneratorService } from './../water-drop-generator.service';

@Component({
  selector: 'app-heatmap',
  templateUrl: './heatmap.component.html',
  styleUrls: ['./heatmap.component.css']
})
export class HeatmapComponent implements OnChanges, OnDestroy, AfterViewInit {

  data: any[];
  heatmap: any;
  chart: ChartXY;
  fill: PalettedFill;
  palette: LUT;

  //engineOptions: EngineOptions;

  //chartId: number;

  constructor(private dataService: DataService, private waterDropGenerator: WaterDropGeneratorService) {
    this.palette = new LUT({
      steps: [
        { value: 0, color: ColorRGBA(0, 0, 0) },
        { value: 30, color: ColorRGBA(255, 255, 0) },
        { value: 45, color: ColorRGBA(255, 204, 0) },
        { value: 60, color: ColorRGBA(255, 128, 0) },
        { value: 100, color: ColorRGBA(255, 0, 0) }
      ],
      interpolate: false
    });

    this.fill = new PalettedFill({ lut: this.palette });
  }

  ngOnChanges(): void {
    // Generate random ID to us as the containerId for the chart and the target div id
    //this.chartId = Math.trunc(Math.random() * 1000000);
  }

  ngAfterViewInit() {

    const resolutionX: number = 200;
    const resolutionY: number = 200;

    //let containerid: string = `${this.chartId}`;

    //console.log("After View Init Container ID: " + containerid);

    //this.engineOptions = {container: containerid};

    // Create chartXY
    this.chart = lightningChart().ChartXY(
      {
        container: "lcjsc",
        theme: Themes.dark,
        height: 400,
        width: 600
      });
    // Set chart title
    this.chart.setTitle('Getting Started');

    this.data = this.waterDropGenerator.WaterDropGenerator(
      resolutionX,      // size of nodes in X
      resolutionY,      // size of nodes in Z
      [0.2, 0.5, 0.7],  // Drop X positions in scale 0...1
      [0.6, 0.5, 0.3],  // Drop Z positions in scale 0...1
      [15, 50, 3],      // Amplitudes, as Y axis values
      47,               // Offset level (mid-Y)
      25                // Volatility, wave generating density
    );

    this.heatmap = this.chart.addHeatmapSeries({
      rows: resolutionX,
      columns: resolutionX,
      start: { x: 10, y: 10 },
      end: { x: 90, y: 90 },
      pixelate: false
    });
    this.heatmap.invalidateValuesOnly(this.data);
    this.heatmap.setFillStyle(this.fill);
  }

  ngOnDestroy() {
    // "dispose" should be called when the component is unmounted to free all the resources used by the chart
    this.chart.dispose();
  }
}
