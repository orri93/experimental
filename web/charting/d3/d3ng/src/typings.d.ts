/**
 * This is a simple typings definition for Emscripten's Module object
 */
interface EmscriptenReadFileOptions {
  encoding?: "utf8" | "binary";
  flags?: string;
}

interface CcallOptions {
  async?: boolean;
}

interface EmscriptenModule {
  arguments?: string[];
  print?(what: string): void;
  printErr?(what: string): void;
  locateFile?(file: string): string;
  ccall?(funcName: string, returnType: string, argumentTypes: string[], arguments: any[], options?: CcallOptions): any;
  preRun?: Function[];
  postRun?: Function[];
  canvas?: HTMLCanvasElement;
  FS_createDataFile?(
    parent: string,
    name: string,
    data: string | Uint8Array,
    canRead?: boolean,
    canWrite?: boolean,
    canOwn?: boolean
  ): void;
  FS_createPreloadedFile?(parent: string, name: string, url: string, canRead?: boolean, canWrite?: boolean): void;
  FS_readFile?(url: string, options?: EmscriptenReadFileOptions): any;
  FS_unlink?(path: string): void;
}

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

interface ChartAxes {
  x: number,
  y: number,
  width: number;
  height: number;
}

interface ChartConfiguration {
  width: number;
  height: number;
}

interface HeatmapConfiguration {
  chart: ChartConfiguration;
  range: DataRange;
  axes: ChartAxes;
}

interface ChartTimer {
  interval: number;
}

interface ApplicationConfiguration {
  heatmap: HeatmapConfiguration;
  timer: ChartTimer;
}
