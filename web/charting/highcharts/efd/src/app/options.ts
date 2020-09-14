export interface TextOption {
  text: string;
}

export interface EnabledOption {
  enabled: boolean;
}

export interface Boost {
  allowForce?: boolean;
  useGPUTranslations?: boolean;
}

export interface Legend {
  x?: number;
  y?: number;
  align?: string;
  verticalAlign?: string;
  layout?: string;
  symbolHeight?: number;
}

export interface PlotOptions {
  colsize?: number;
  rowsize?: number;
  series: any;
  heatmap: any;
}

export interface XAxis {
  title?: {
    align?: string,
    text: string,
  };
  type?: string;
  reversed?: boolean;
  labels?: {
    format?: string,
    formatter?: any,
    style?: {
      color?: string,
    },
  };
  tickLength?: number;
  tickInterval?: number;
  min?: number | Date;
  max?: number | Date;
  minPadding?: number;
  maxPadding?: number;
  events?: {
    afterSetExtremes?: any,
  };
}

export interface YAxis {
  title?: {
    align?: string,
    text: string,
  };
  type?: string;
  gridLineColor?: string;
  startOnTick?: boolean;
  endOnTick?: boolean;
  tickWidth?: number;
  lineWidth?: number;
  min?: number;
  max?: number;
  minPadding?: number;
  maxPadding?: number;
  reversed?: boolean;
  events?: {
      afterSetExtremes?: any,
  };
}

export interface ColorAxis {
  stops?: Array<[number, string]>;
  min?: number;
  max?: number;
  minPadding?: number;
  maxPadding?: number;
  startOnTick?: boolean;
  endOnTick?: boolean;
  labels?: {
    enabled?: boolean,
    formatter?: any,
    style?: {
      color?: string,
    }
  };
  tickPositioner: any;
  reversed?: boolean;
}

export interface Chart {
  animation?: boolean;
  marginLeft?: number;
  type?: string;
}

export interface Options {
  chart?: Chart;
  tooltip?: EnabledOption;
  boost?: Boost;
  title?: TextOption;
  credits?: EnabledOption;
  exporting?: EnabledOption;
  xAxis?: XAxis;
  yAxis?: YAxis;
  colorAxis?: ColorAxis;
  legend?: Legend;
  plotOptions?: PlotOptions;
  series?: Array<any>;
}
