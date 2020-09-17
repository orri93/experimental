/*
 *
 * LightningChartJS example that showcases a simple XY line series.
 *
 */

 // Import LightningChartJS
const lcjs = require('@arction/lcjs');

var NUMBER_OF_ROWS = 400;
var NUMBER_OF_COLUMNS = 600;
var MINIMUM_COLOR_INIT = 0;
var MINIMUM_COLOR_LINE = 0;
var MAXIMUM_COLOR_INIT = 128;
var MAXIMUM_COLOR_LINE = 32;
var TIMER_INTERVAL = 1000;

var data = [];
var myTimer;
var series;
var atcol = 0;

var started = false;

function getRandomInt(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  //The maximum is exclusive and the minimum is inclusive
  return Math.floor(Math.random() * (max - min)) + min;
}

// Init data points (number of points as an argument)
var initData = function(cols, rows) {
  data = [];
  for (i = 0; i < cols; i++) {
    for (j = 0; j < rows; j++) {
      data.push([i, j, getRandomInt(MINIMUM_COLOR_INIT, MAXIMUM_COLOR_INIT)])
    }
  }
  atcol = cols;
}

// Extract required parts from LightningChartJS.
const {
  lightningChart,
  IntensitySeriesTypes,
  PalettedFill,
  LUT,
  emptyFill,
  emptyLine,
  AxisScrollStrategies,
  AxisTickStrategies,
  ColorHSV,
  Themes
} = lcjs;

// Initialize LightningChart JS
const lc = lightningChart();

/**
 * Create a chart for a channel
 * @param {lcjs.Dashboard}  dashboard       Dashboard to create the chart in
 * @param {number}          channelIndex    Current channel index
 * @param {number}          rows            Data row count
 * @param {number}          columns         Data column count
 * @param {number}          maxFreq         Maximum frequency for data
 * @param {number}          duration        Duration in seconds
 */
const createChannel = (dashboard, channelIndex, rows, columns, maxFreq, duration) => {
  // Create a new chart in a specified row
  const chart = dashboard.createChartXY({
    columnIndex: 0,
    columnSpan: 1,
    rowIndex: channelIndex,
    rowSpan: 1
  })
    // Hide the chart title
    .setTitleFillStyle(emptyFill);

  // Start position of the heatmap
  const start = {
    x: 0,
    y: 0
  };
  // End position of the heatmap
  const end = {
    x: duration,
    // Use half of the fft data range
    y: (Math.ceil(maxFreq / 2))
  };
  // Create the series
  const series = chart.addHeatmapSeries({
    // Data columns, defines horizontal resolution
    columns: columns,
    // Data rows, defines vertical resolution
    // Use half of the fft data range
    rows: Math.ceil(rows / 2),
    // Start position, defines where one of the corners for hetmap is
    start,
    // End position, defines the opposite corner of the start corner
    end,
    // Smoothly render the heatmap data
    pixelate: true,
    // Using IntensityGrid, it supports rectangular heatmaps and is simpler than the IntensityMesh type
    type: IntensitySeriesTypes.Grid
  })
    // Use palletted fill style, intensity values define the color for each data point based on the LUT
    .setFillStyle(new PalettedFill({
      lut: new LUT({
        steps: [
          { value: 0, color: ColorHSV(0, 1, 0) },
          { value: 255 * (1 / 6), color: ColorHSV(270, 0.84, 0.2) },
          { value: 255 * (2 / 6), color: ColorHSV(289, 0.86, 0.35) },
          { value: 255 * (3 / 6), color: ColorHSV(324, 0.97, 0.56) },
          { value: 255 * (4 / 6), color: ColorHSV(1, 1, 1) },
          { value: 255 * (5 / 6), color: ColorHSV(44, 0.64, 1) }
        ],
        interpolate: true
      })
    }));

  // Set default X axis settings
  series.axisX.setInterval(start.x, end.x)
    .setTickStrategy(AxisTickStrategies.Empty)
    .setTitleMargin(0)
    .setScrollStrategy(undefined)
    .setMouseInteractions(false);
  // Set default chart settings
  chart.setPadding({ left: 0, top: 8, right: 8, bottom: 1 })
    .setMouseInteractions(false);
  // Set default X axis settings
  series.axisY.setInterval(start.y, end.y)
    .setTitle(`Channel ${channelIndex + 1} (Hz)`)
    .setScrollStrategy(AxisScrollStrategies.fitting);

  return {
    chart,
    series
  };
}

function showLcJsHeatmap() {

}

(async () => {

  console.log("Asyncing");

  // Remove loading spinner
  document.querySelectorAll('.loading').forEach(item => {
    item.parentElement.removeChild(item);
  });

  const run = async () => {
    // Load waveform from url
    // Process the loaded wave form to prepare it for being added to the chart
    const processed = await processWaveForm(waveform);
    // Create a dashboard from the processed waveform data
    const dashboard = renderSpectrogram(processed);
  }

  // Check if audio context was started
  if (started) {    
    run();
  } else {

    const startElement = document.createElement("div");
    const startButton = document.createElement("button");
    startButton.setAttribute("onClick", "showLcJsHeatmap()");
    startButton.innerHTML = "Show";
    startElement.appendChild(startButton);

    const innerElement = document.querySelector('.inner');
    let target;
    if (!innerElement) {
      target = document.createElement('div');
      target.classList.add('inner');
      document.body.appendChild(target);
    }
    const targetElement = innerElement || target;

    targetElement.appendChild(startElement);
  }

})()
