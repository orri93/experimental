import { Injectable } from '@angular/core';

@Injectable({providedIn: 'root'})
export class WaterDropGeneratorService {

  oscillators: any[] = [];

  constructor() { }

  /**
   * Calculate waves at given point.
   * @param x wave origin x-coordinate.
   * @param z wave origin z-coordinate.
   * @param volatility Volatility.
   * @returns wave height at given point.
   */
  CalculateWavesAtPoint(x: number, z: number, volatility: number): number {
    let resultValue: number = 0;
    const iOscillatorCount = this.oscillators.length;
    for (let i = 0; i < iOscillatorCount; i++) {
      const oscillator =  this.oscillators[i];
      const distX = x - oscillator.centerX;
      const distZ = z - oscillator.centerZ;
      const dist = Math.sqrt(distX * distX + distZ * distZ);
      resultValue += oscillator.gain * oscillator.amplitude * Math.cos(dist * volatility) * Math.exp(-dist * 3.0);
    }
    return resultValue;
  }

  /**
   * Generate water drop surface data.
   * @param sizeX Size of output array X dimensions.
   * @param sizeZ Size of output array Z dimensions.
   * @param xPositionsNormalized X positions of drops
   * @param zPositionsNormalized Z positions of drops
   * @param amplitudes Amplitudes
   * @param offsetLevel Offset level
   * @param volatility Volatility, set larger number to generate more waves for each drop
   */
  WaterDropGenerator(
    sizeX: number,
    sizeZ: number,
    xPositionsNormalized: number[],
    zPositionsNormalized: number[],
    amplitudes: number[],
    offsetLevel: number,
    volatility: number
  ): any[] {
    const iOscCount = amplitudes.length;

    for (let iOsc = 0; iOsc < iOscCount; iOsc++) {
      this.oscillators[iOsc] = {
        amplitude: amplitudes[iOsc],
        centerX: xPositionsNormalized[iOsc],
        centerZ: zPositionsNormalized[iOsc],
        gain: 1,
        offsetY: 0
      }
    }

    const result = Array.from(Array(sizeZ)).map(() => Array(sizeX));
    const dTotalX = 1;
    const dTotalZ = 1;
    const stepX = (dTotalX / sizeX);
    const stepZ = (dTotalZ / sizeZ);

    // calculate the data
    for (let row = 0, z = 0; row < sizeZ; row++, z += stepZ) {
      for (let col = 0, x = 0; col < sizeX; col++, x += stepX) {
        result[col][row] =
          this.CalculateWavesAtPoint(x, z, volatility) +
          offsetLevel;
      }
    }

    return result;
  }
}
