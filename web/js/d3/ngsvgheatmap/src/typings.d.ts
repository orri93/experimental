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

interface SvgHeatmapConfiguration {
  chart: ChartConfiguration;
  range: DataRange;
}

interface CanvasHeatmapConfiguration {
  chart: ChartConfiguration;
  range: DataRange;
}

interface ApplicationConfiguration {
  svg: SvgHeatmapConfiguration;
  canvas: CanvasHeatmapConfiguration;
}
