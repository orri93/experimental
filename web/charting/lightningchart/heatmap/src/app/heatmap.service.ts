import { Injectable } from '@angular/core';
import { PalettedFill, ColorRGBA, LUT } from '@arction/lcjs';

@Injectable({providedIn: 'root'})
export class HeatmapService {

  palette: LUT;
  fill: PalettedFill;

  paletteInterpolated: LUT;
  fillInterpolated: PalettedFill;

  constructor() {
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

    this.paletteInterpolated = new LUT({
      steps: [
        { value: 0, color: ColorRGBA(0, 0, 0) },
        { value: 30, color: ColorRGBA(255, 255, 0) },
        { value: 45, color: ColorRGBA(255, 204, 0) },
        { value: 60, color: ColorRGBA(255, 128, 0) },
        { value: 100, color: ColorRGBA(255, 0, 0) }
      ],
      interpolate: true
    });

    this.fill = new PalettedFill({ lut: this.palette });
    this.fillInterpolated = new PalettedFill({ lut: this.paletteInterpolated });
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
