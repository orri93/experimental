export interface XAxis {
  title?: {
    align?: string,
    text: string,
  },
  type?: string,
  reversed?: boolean,
  labels?: {
    format?: string,
    formatter?: any,
    style?: {
      color?: string,
    },
  },
  tickLength?: number,
  tickInterval?: number,
  min?: number | Date,
  max?: number | Date,
  minPadding?: number,
  maxPadding?: number,
  events?: {
    afterSetExtremes?: any,
  },
}
