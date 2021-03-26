import { Mock, Range } from './m5.model';

export interface M5ModuleConfiguration {
  size: ChartSize;
  axes: ChartAxes;
  legend: ChartLegend;
  formatting: Formatting;
  intervals: number;
  padding: number;
  count: number;
  range: Range;
  mock: Mock;
}
