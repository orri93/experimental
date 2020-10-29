import { interval, Subscription } from "rxjs";
import { NumericAxis } from "scichart/Charting/Visuals/Axis/NumericAxis";
import { SciChartSurface } from "scichart";
import { UniformHeatmapDataSeries } from "scichart/Charting/Model/UniformHeatmapDataSeries";
import { UniformHeatmapRenderableSeries } from "scichart/Charting/Visuals/RenderableSeries/UniformHeatmapRenderableSeries";
import { HeatmapColorMap } from "scichart/Charting/Visuals/RenderableSeries/HeatmapColorMap";
import { zeroArray2D } from "scichart/utils/zeroArray2D";
import { NumberRange } from "scichart/Core/NumberRange";

const TIMER_INTERVAL = 10.0;

const MINIMUM_Z = 0.0;
const MAXIMUM_Z = 100.0;

const REAL_TIME_MINIMUM_Z = 0.0;
const REAL_TIME_MAXIMUM_Z = 50.0;

const RES_X = 600.0;
const RES_Y = 400.0;

const MARKER_NAME_LOAD_START = "load-start";
const MARKER_NAME_LOAD_COMPLETED = "load-completed";
const MEASURE_LOAD = "load-measure";

const MARKER_NAME_TIMER_START = "timer-start";
const MARKER_NAME_TIMER_COMPLETED = "timer-completed";
const MEASURE_TIMER = "timer-measure";

const MARKER_NAME_INTERVAL_START = "interval-start";
const MARKER_NAME_INTERVAL_COMPLETED = "interval-completed";
const MEASURE_INTERVAL = "interval-measure";

function getRandomInteger(minimum, maximum) {
  minimum = Math.ceil(minimum);
  maximum = Math.floor(maximum);
  // The maximum is exclusive and the minimum is inclusive
  return Math.floor(Math.random() * (maximum - minimum)) + minimum;
}

function generateLine(rows, minimum, maximum) {
  // Helper function which creates an empty 2D array filled with zeros
  const zValues = zeroArray2D([rows, 1]);
  for (let y = 0; y < rows; y++) {
    zValues[y][0] = getRandomInteger(minimum, maximum);
  }
  return zValues;
}

// This function generates data for the heatmap series example
// because data-generation is not trivial, we generate once before the example starts
// so you can see the speed & power of SciChart.js
function generateData(columns, rows, minimum, maximum) {
  // Helper function which creates an empty 2D array filled with zeros
  const zValues = zeroArray2D([rows, columns]);
  // fill the 2D array with values.
  for (let x = 0; x < columns; x++) {
    for (let y = 0; y < rows; y++) {
      zValues[y][x] = getRandomInteger(minimum, maximum);
    }
  }
  return zValues;
}

async function initSciChart() {
  // Below find a trial / BETA key for SciChart.js.
  // This Expires in 30 days - or 14th November 2020
  // Set this license key once in your app before calling SciChartSurface.create, e.g.
  SciChartSurface.setRuntimeLicenseKey("WcnXtRLwGVtfNA59XwvDQA11wSpykEA1NEpARELTB+Aq6kf2nJSK9GgWOKvCJA6P+jNg2xcVLw3oM7EdIIi0MJtvorAARa9au01LV/xLJ1jdOeDeMXpw/eT5ajSpukKcJXHe97tzsBzfB6wRziW6LgNjuB3ykFIk+tGvOmJyhRewYjF+FCSb/0q8Bq8em4lNmOfONzJz5spVWvvfHdn5iIYfvv00hhduow4bFzxXnRucLtHl2Bm1yFvrVYe0UOQcFpJ9DZ4S96GLhSw9SIkUSAy/C5r3FvdCkX8d40ehAg+n78w92QXwh4B41xF0f+9OHpeV3byaZDNr5L1afdS3qCahoyeYEnmt4hYdmGH3uS+KtC29bAcVXUqNA9P3pESndALjlEimVNfr6RrfKEY3jroWtPXEx2Oo9XcD3ZLUJiRrjDL0lTf/3a6+KN1xsl2K2eymqyo9Wggy7Mf3WymmvURil7SaxE3xBP5LWWGPMEXvf9m7vXGz6fkEtsZhdEC3HQprBwEGyV1zPdLxDqtWO9ltEBEBlS2FrzJ3984/zSp9sbc=");

  performance.mark(MARKER_NAME_LOAD_START);

  // Create the SciChartSurface in the div 'scichart-root'
  // The SciChartSurface, and webassembly context 'wasmContext' are paired. This wasmContext
  // instance must be passed to other types that exist on the same surface.
  const {sciChartSurface, wasmContext} = await SciChartSurface.create("scichart-root");

  // Create an X,Y Axis and add to the chart
  const xAxis = new NumericAxis(wasmContext);
  const yAxis = new NumericAxis(wasmContext);
  
  sciChartSurface.xAxes.add(xAxis);
  sciChartSurface.yAxes.add(yAxis);

  // Generate some data for the heatmap series. We require 2-dimensional array of numbers (number[][] in Typescript)
  let values = generateData(RES_X, RES_Y, MINIMUM_Z, MAXIMUM_Z);
  const heatmapDataSeries = new UniformHeatmapDataSeries(
    wasmContext,
    100,
    1,
    100,
    1,
    values);
  
  // Create a Heatmap RenderableSeries with the color map. ColorMap.minimum/maximum defines the values in
  // HeatmapDataSeries which correspond to gradient stops at 0..1
  const heatmapSeries = new UniformHeatmapRenderableSeries(wasmContext, {
    dataSeries: heatmapDataSeries,
    colorMap: new HeatmapColorMap({
      minimum: MINIMUM_Z,
      maximum: MAXIMUM_Z,
      gradientStops: [
        { offset: 0.00, color: "#07049b" },   /* Dark blue */
        { offset: 0.30, color: "#02f7f3" },   /* Cyan      */
        { offset: 0.35, color: "#09f725" },   /* Green     */
        { offset: 0.50, color: "#f4ec04" },   /* Yellow    */
        { offset: 0.70, color: "#f79d01" },   /* Orange    */
        { offset: 1.00, color: "#8c0101" },   /* Dark red  */
      ],
    }),
  });

  // Add heatmap to the chart
  sciChartSurface.renderableSeries.add(heatmapSeries);

  performance.mark(MARKER_NAME_LOAD_COMPLETED);
  performance.measure(MEASURE_LOAD, MARKER_NAME_LOAD_START, MARKER_NAME_LOAD_COMPLETED);

  var performanceDivElement = document.getElementById("performance-load-result-id");
  var entries = performance.getEntriesByType("measure");
  var firstentry = entries[0];

  performanceDivElement.innerHTML = "Load time: " + firstentry.duration + " ms";

  performance.clearMarks();
  performance.clearMeasures();

  let timer = null;
  let firstInterval = false;

  let timerCount = 0.0;
  let timerSum = 0.0;

  let intervalCount = 0.0;
  let intervalSum = 0.0;

  const onTimer = () => {
    if(!firstInterval) {
      performance.mark(MARKER_NAME_INTERVAL_COMPLETED);
      performance.measure(MEASURE_INTERVAL, MARKER_NAME_INTERVAL_START, MARKER_NAME_INTERVAL_COMPLETED);
      let performanceIntervalDivElement = document.getElementById("performance-interval-result-id");
      var entriesInterval = performance.getEntriesByType("measure");
      var firstIntervalEntry = entriesInterval[0];

      var intervalDuration = firstIntervalEntry.duration;
      intervalCount++;
      intervalSum += intervalDuration;
      var averageInterval = intervalSum / intervalCount;

      performanceIntervalDivElement.innerHTML = "Interval. Last: " + intervalDuration + " ms, Average: " + averageInterval + " ms, Count: " + intervalCount;

      performance.clearMarks();
      performance.clearMeasures();
    } else {
      console.log("First interval");
      firstInterval = false;
    }

    performance.mark(MARKER_NAME_TIMER_START);
    performance.mark(MARKER_NAME_INTERVAL_START);

    // Work
    //values = generateData(RES_X, RES_Y, REAL_TIME_MINIMUM_Z, REAL_TIME_MAXIMUM_Z);
    const line = generateLine(RES_Y, REAL_TIME_MINIMUM_Z, REAL_TIME_MAXIMUM_Z);
    let i = 0;
    values.forEach((v) => {
      v.shift();
      v.push(line[i++]);
    });
    heatmapDataSeries.setZValues(values);
    // Work done

    performance.mark(MARKER_NAME_TIMER_COMPLETED);
    performance.measure(MEASURE_TIMER, MARKER_NAME_TIMER_START, MARKER_NAME_TIMER_COMPLETED);

    var performanceTimerDivElement = document.getElementById("performance-timer-result-id");
    var entriesTimer = performance.getEntriesByType("measure");
    var firstTimerEntry = entriesTimer[0];

    var timerDuration = firstTimerEntry.duration;
    timerCount++;
    timerSum += timerDuration;
    var average = timerSum / timerCount;

    performanceTimerDivElement.innerHTML = "Timer. Last: " + timerDuration + " ms, Average: " + average + " ms, Count: " + timerCount;

    performance.clearMeasures();
  }

  document.getElementById("start-button").onclick = function() {
    let timerInterval = TIMER_INTERVAL;
    console.log(`Start button clicked with timer interval as ${timerInterval}`);
    if(timer == null || timer.closed) {
      firstInterval = true;
      timerCount = 0.0;
      timerSum = 0.0;
      intervalCount = 0.0;
      intervalSum = 0.0;
      timer = interval(timerInterval).subscribe(( x => {
        onTimer();
      }));
      console.log("Starting timer subscription.");
    } else {
      console.log("Timer subscription already started.");
    }
  };

  document.getElementById("stop-button").onclick = function() {
    console.log("Stop Real Time button clicked");
    if(timer.closed) {
      console.log("Timer already closed.");
    } else {
      console.log("Unsubscribe the timer.");
      timer.unsubscribe();
    }
  };
}

initSciChart();
