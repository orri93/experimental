interface Point {
  x: number;
  y: number;
}

interface NumberRange {
  from: number;
  to: number;
}

interface GraphRange {
  x: NumberRange;
  y: NumberRange;
}

interface GraphTickValues {
  x: number[],
  y: number[]
}

interface GraphSize {
  width: number;
  height: number;
}
