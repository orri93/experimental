/**
 * Typing definition for the Application Configuration
 */

interface TimeUpdate {
  interval: number;
}

interface EcdUpdate {
  interval: number;
}

interface UpdateService {
  time: TimeUpdate;
  ecd: EcdUpdate;
}

interface TagService {
  interval: number;
}

interface MockService {
  update: UpdateService;
  tag: TagService;
}

interface Intervals {
  initialization: number;
  displayCurrent: number;
  timer: number;
}

interface WasmHeatmapPageConfiguration {
  size: ChartSize;
  range: DataRange;
  axes: ChartAxes;
  parsing: Parsing;
  formatting: Formatting;
  intervals: Intervals;
}

interface ApplicationConfiguration {
  wasm: WasmHeatmapPageConfiguration;
  services: MockService;
}
