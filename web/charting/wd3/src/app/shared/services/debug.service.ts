import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class DebugService {

  constructor() { }

  static formatSizeLog(size: ChartSize): string {
    return `width: ${size.width}, height: ${size.height}`;
  }

  static consoleLogSize(size: ChartSize, message: string): void {
      console.log(message + ': { ' + DebugService.formatSizeLog(size) + ' }');
  }
}
