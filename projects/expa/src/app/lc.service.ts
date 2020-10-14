import { Injectable } from '@angular/core';
import {
  Themes,
  PalettedFill,
  EngineOptions,
  DashboardOptions,
  ChartXYOptions,
  ColorRGBA,
  LUTStep,
  LUT } from '@arction/lcjs';

@Injectable({ providedIn: 'root' })
export class LcService {

  steps: LUTStep[];

  palette: LUT;
  fill: PalettedFill;

  paletteInterpolated: LUT;
  fillInterpolated: PalettedFill;

  constructor() {
    console.log("Construct the LC Service");
    this.steps = [
      { value: 0x00, color: ColorRGBA(0x07, 0x04, 0x9b) },  /*   0 Dark blue */
      { value: 0x1e, color: ColorRGBA(0x02, 0xf7, 0xf3) },  /*  30 Cyan      */
      { value: 0x23, color: ColorRGBA(0x09, 0xf7, 0x25) },  /*  35 Green     */
      { value: 0x32, color: ColorRGBA(0xf4, 0xec, 0x04) },  /*  50 Yellow    */
      { value: 0x46, color: ColorRGBA(0xf7, 0x9d, 0x01) },  /*  70 Orange    */
      { value: 0x64, color: ColorRGBA(0x8c, 0x01, 0x01) }   /* 100 Dark red  */
    ];

    this.palette = new LUT({
      steps: this.steps,
      interpolate: false
    });

    this.paletteInterpolated = new LUT({
      steps: this.steps,
      interpolate: true
    });

    this.fill = new PalettedFill({ lut: this.palette });
    
    this.fillInterpolated = new PalettedFill({ lut:this.paletteInterpolated });

    console.log("LC Service ready");
  }

  createDashboardOptions(height: number, width: number): EngineOptions & DashboardOptions {
    return {
      container: "lcjsc",
      numberOfRows: 1,
      numberOfColumns: 2,
      theme: Themes.dark,
      height: height,
      width: width
    };
  }

  createChartOptions(height: number, width: number): EngineOptions & ChartXYOptions {
    return {
      container: "lcjsc",
      theme: Themes.dark,
      height: height,
      width: width
    };
  }

  getPalette(interpolate: boolean): LUT {
    if(interpolate) {
      return this.paletteInterpolated;
    } else {
      return this.palette;
    }
  }

  getFill(interpolate: boolean): PalettedFill {
    if(interpolate) {
      return this.fillInterpolated;
    } else {
      return this.fill;
    }
  }
}
