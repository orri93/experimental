import { Component, AfterViewInit, OnDestroy } from '@angular/core';
import { lightningChart, PalettedFill, ChartXY, Themes, Point } from '@arction/lcjs';
import { HeatmapService } from './../heatmap.service';
import { WaterDropGeneratorService } from './../water-drop-generator.service';

const WATER_DROP_FIRST_X = 10.0;
const WATER_DROP_FIRST_Y = 10.0;

const WATER_DROP_LAST_X = 90.0;
const WATER_DROP_LAST_Y = 90.0;

const WATER_DROP_RES_X = 200.0;
const WATER_DROP_RES_Y = 200.0;

const WATER_DROP_SIZE_WIDTH = 600.0;
const WATER_DROP_SIZE_HEIGHT = 600.0;

const WATER_DROP_INTERPOLATED_PALLET = true;
const WATER_DROP_PIXELATE = false;

@Component({
  selector: 'app-water-drop',
  templateUrl: './water-drop.component.html',
  styleUrls: ['./water-drop.component.css']
})
export class WaterDropComponent implements OnDestroy, AfterViewInit {

  data: any[];
  heatmap: any;
  chart: ChartXY;
  start: Point;
  end: Point;

  private createWaterDrop(resx: number, resy: number): void {
    this.data = this.waterDropGenerator.waterDropGenerator(
      resx,             // size of nodes in X
      resy,             // size of nodes in Z
      [0.2, 0.5, 0.7],  // Drop X positions in scale 0...1
      [0.6, 0.5, 0.3],  // Drop Z positions in scale 0...1
      [15, 50, 3],      // Amplitudes, as Y axis values
      47,               // Offset level (mid-Y)
      25                // Volatility, wave generating density
    );
  }

  constructor(private heatmapService: HeatmapService, private waterDropGenerator: WaterDropGeneratorService) {
    this.start = { x: WATER_DROP_FIRST_X, y: WATER_DROP_FIRST_Y };
    this.end = {x: WATER_DROP_LAST_X, y: WATER_DROP_LAST_Y };
  }

  ngAfterViewInit() {

    const fill: PalettedFill = this.heatmapService.getFill(WATER_DROP_INTERPOLATED_PALLET);

    // Create chartXY
    this.chart = lightningChart().ChartXY({
      container: "lcjsc-water-drop",
      theme: Themes.dark,
      height: WATER_DROP_SIZE_HEIGHT,
      width: WATER_DROP_SIZE_WIDTH
    });

    // Set chart title
    this.chart.setTitle('Waterdrop');

    this.createWaterDrop(WATER_DROP_RES_X, WATER_DROP_RES_Y);

    this.heatmap = this.chart.addHeatmapSeries({
      rows: WATER_DROP_RES_Y,
      columns: WATER_DROP_RES_X,
      start: this.start,
      end: this.end,
      pixelate: WATER_DROP_PIXELATE
    });
    this.heatmap.invalidateValuesOnly(this.data);
    this.heatmap.setFillStyle(fill);
  }

  ngOnDestroy() {
    // "dispose" should be called when the component is unmounted to free all the resources used by the chart
    this.chart.dispose();
  }
}
