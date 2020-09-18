import { Component, AfterViewInit, OnDestroy } from '@angular/core';
import { lightningChart, PalettedFill, ChartXY, Themes, Point } from '@arction/lcjs';
import { HeatmapService } from './../heatmap.service';
import { DataService } from './../data.service';

const MINIMUM_Z = 0.0;
const MAXIMUM_Z = 64.0;

const FIRST_X = 0.0;
const FIRST_Y = 0.0;

const LAST_X = 600.0;
const LAST_Y = 400.0;

const RES_X = 600.0;
const RES_Y = 400.0;

const SIZE_WIDTH = 600.0;
const SIZE_HEIGHT = 400.0;

const INTERPOLATED_PALLET = true;
const PIXELATE = false;

@Component({
  selector: 'app-heatmap',
  templateUrl: './heatmap.component.html',
  styleUrls: ['./heatmap.component.css']
})
export class HeatmapComponent implements OnDestroy, AfterViewInit {

  data: any[];
  heatmap: any;
  chart: ChartXY;
  start: Point;
  end: Point;

  private createRandom(resx: number, resy: number): void {
    this.data = this.dataService.dataGenerator(resx, resy, MINIMUM_Z, MAXIMUM_Z);
  }

  constructor(private heatmapService: HeatmapService, private dataService: DataService) {
    this.start = { x: FIRST_X, y: FIRST_Y };
    this.end = {x: LAST_X, y: LAST_Y };
  }

  ngAfterViewInit() {

    const fill: PalettedFill = this.heatmapService.getFill(INTERPOLATED_PALLET);

    // Create chartXY
    this.chart = lightningChart().ChartXY(
      {
        container: "lcjsc",
        theme: Themes.dark,
        height: SIZE_HEIGHT,
        width: SIZE_WIDTH
      });

    // Set chart title
    this.chart.setTitle('Random Data');

    this.createRandom(RES_X, RES_Y);

    this.heatmap = this.chart.addHeatmapSeries({
      rows: RES_Y,
      columns: RES_X,
      start: this.start,
      end: this.end,
      pixelate: PIXELATE
    });
    this.heatmap.invalidateValuesOnly(this.data);
    this.heatmap.setFillStyle(fill);
  }

  ngOnDestroy() {
    // "dispose" should be called when the component is unmounted to free all the resources used by the chart
    this.chart.dispose();
  }
}
