/**
 * Typing definition for the Application Configuration
 */
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

interface ApplicationConfiguration {
  bar: BarConfiguration;
  pie: PieConfiguration;
  scatter: ScatterConfiguration;
}
