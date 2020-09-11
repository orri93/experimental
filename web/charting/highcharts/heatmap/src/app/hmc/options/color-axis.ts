export interface ColorAxis {
  stops?: Array<[number, string]>,
  min?: number,
  max?: number,
  minPadding?: number,
  maxPadding?: number,
  startOnTick?: boolean,
  endOnTick?: boolean,
  labels?: {
    enabled?: boolean,
    formatter?: any,
    style?: {
      color?: string,
    }
  },
  tickPositioner: any,
  reversed?: boolean
}
