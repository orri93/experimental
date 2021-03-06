/**
 * Typing definition for the Application Configuration
 */

interface RangeItem {
  from: number;
  to: number;
}

interface DataRange {
  x: RangeItem;
  y: RangeItem;
  z: RangeItem;
}

interface ChartConfiguration {
  width: number;
  height: number;
  margin: number;
}

interface BarConfiguration {
  chart: ChartConfiguration;
}

interface PieConfiguration {
  chart: ChartConfiguration;
}

interface ScatterConfiguration {
  chart: ChartConfiguration;
}

interface HeatmapConfiguration {
  chart: ChartConfiguration;
  range: DataRange;
}

interface ApplicationConfiguration {
  bar: BarConfiguration;
  pie: PieConfiguration;
  scatter: ScatterConfiguration;
  heatmap: HeatmapConfiguration;
}
