export interface YAxis {
  title?: {
    align?: string,
    text: string,
  },
  type?: string,
  gridLineColor?: string,
  startOnTick?: boolean,
  endOnTick?: boolean,
  tickWidth?: number,
  lineWidth?: number,
  min?: number,
  max?: number,
  minPadding?: number,
  maxPadding?: number,
  reversed?: boolean,
  events?: {
      afterSetExtremes?: any,
  },
}
