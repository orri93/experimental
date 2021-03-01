/**
 * General types
 */

interface NumberRange {
  from: number;
  to: number;
}

interface MatrixRange {
  columnCount: number;
  rowCount: number;
}

interface ChartSize {
  width: number;
  height: number;
}

interface ChartAxes {
  x: number;
  y: number;
  size: ChartSize;
  color?: string;
}

interface DataRange {
  x: NumberRange;
  y: NumberRange;
  z: NumberRange;
}

interface Parsing {
  dateTime: string;
}

interface Formatting {
  time: string;
}
