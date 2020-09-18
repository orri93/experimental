import { Injectable } from '@angular/core';

@Injectable({providedIn: 'root'})
export class DataService {

  private colat: number = 0;

  constructor() { }

  private getRandomInteger(minimum: number, maximum: number): number {
    minimum = Math.ceil(minimum);
    maximum = Math.floor(maximum);
    // The maximum is exclusive and the minimum is inclusive
    return Math.floor(Math.random() * (maximum - minimum)) + minimum;
  }

  createLine(x: number, count: number, minimum: number, maximum: number): any[] {
    let dataline: any[] = [];
    for(let i: number = 0; i < count; i++) {
      dataline.push([x, i, this.getRandomInteger(minimum, maximum)]);
    }
    return dataline;
  }

  dataGenerator(columns: number, rows: number, minimum: number, maximum: number): any[] {
    const result = Array.from(Array(rows)).map(() => Array(columns))
    for(let row = 0; row < rows; row++) {
      for(let col = 0; col < columns; col++) {
        result[row][col] = this.getRandomInteger(minimum, maximum);
      }
    }
    return result;
  }
}
