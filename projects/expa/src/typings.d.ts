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
interface LcConfiguration {
  interpolate: boolean;
  pixelate: boolean;
}

interface ApplicationConfiguration {
  timerInterval: number;
  lc: LcConfiguration;
}

interface DataPoint {
  x: number;
  y: number;
  z: number;
}

interface InterpolateData {
  x: number;
  y1: number;
  y2: number;
  z1: number;
  z2: number;
  resolution: number;
}

interface Point {
  x: number;
  y: number;
}

interface Vector {
  p: Point[];
}

interface DataRange {
  f: number;
  t: number;
}

interface DataRanges {
  x: DataRange;
  y: DataRange;
}

interface Matrix {
  r: DataRanges;
  v: Vector[];
}

interface WsStart {
  f: number;
}

interface WsUpdate {
  t: number;
  v: Vector;
} 

interface WsMessage {
  t: string;
  m?: string;
  s?: WsStart;
  u?: WsUpdate;
} 
