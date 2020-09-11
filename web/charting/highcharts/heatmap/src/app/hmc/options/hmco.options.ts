import { Hmco } from './hmco.options.datamodel'

export const hmcoOptions: Hmco = {
  chart: {
    animation: false,
    marginLeft: 118,
    type: 'heatmap'
  },
  tooltip: {
    enabled: false,
  },
  boost: {
    allowForce: false,
    useGPUTranslations: true
  },
  title: {
    text: 'ECD'
  },
  credits: {
    enabled: false
  },
  exporting: {
    enabled: false
  },
  xAxis: {
    type: 'datetime',
    tickLength: 10,
    minPadding: 0,
    maxPadding: 0
  },
  yAxis: {
    type: 'linear',
    gridLineColor: 'transparent',
    reversed: true,
    minPadding: 0,
    maxPadding: 0,
    tickWidth: 1,
    lineWidth: 1,
    startOnTick: false,
    endOnTick: false
  },
  colorAxis: {
    stops: [
      [0, '#07049b'],    // Dark blue
      [0.3, '#02f7f3'],  // Cyan
      [0.35, '#09f725'], // Green
      [0.5, '#f4ec04'],  // yellow
      [0.7, '#f79d01'],  // Orange
      [1, '#8c0101']     // Dark red
    ],
    reversed: false,
    labels: {
      enabled: true,
      formatter: function () {
        return Math.round(this.value * 100) / 100;
      },
    },
    tickPositioner: function () {
      let positions = [];
      let delta = (this.max - this.min) / 4;

      positions[0] = this.min;

      for (let i = 1; i < 4; i++) {
        positions[i] = positions[i - 1] + delta;
      }

      positions[4] = this.max;

      return positions;
    },
    startOnTick: false,
    endOnTick: false,
    maxPadding: 0,
  },
  legend: {
    align: 'right',
    verticalAlign: 'middle',
    layout: 'vertical',
    y: 5,
    symbolHeight: 195,
  }
}
