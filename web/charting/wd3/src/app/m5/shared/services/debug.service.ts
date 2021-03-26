import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class DebugService {

  constructor() { }

  static formatSizeLog(size: ChartSize): string {
    return `width: ${size.width}, height: ${size.height}`;
  }

  static formatAxesLog(axes: ChartAxes): string {
    if (axes.color) {
      return `x: ${axes.x}, y: ${axes.y}, size: { ` +
        DebugService.formatSizeLog(axes.size) +
        ` }, color: ${axes.color}`;
    } else {
      return `x: ${axes.x}, y: ${axes.y}, size: { ` +
        DebugService.formatSizeLog(axes.size) +
        ` }`;
    }
  }

  static consoleLogSize(size: ChartSize, message: string): void {
    console.log(message + ': { ' + DebugService.formatSizeLog(size) + ' }');
  }

  static consoleLogAxes(axes: ChartAxes, message: string): void {
    console.log(message + ': { ' + DebugService.formatAxesLog(axes) + ' }');
  }
}
