export class InterpolateData {
  zValue1: number;
  zValue2: number;
  yValue1: number;
  yValue2: number;
  xValue: number;
  resolution: number;

  constructor(xValue: number, yValue1: number, yValue2: number,
    zValue1: number, zValue2: number, resolution: number) {
    this.zValue1 = zValue1;
    this.zValue2 = zValue2;
    this.yValue1 = yValue1;
    this.yValue2 = yValue2;
    this.xValue = xValue;
    this.resolution = resolution;
  }
}
