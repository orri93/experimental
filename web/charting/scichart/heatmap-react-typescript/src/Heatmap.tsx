import React from 'react';
import './Heatmap.css';
import { NumericAxis } from "scichart/Charting/Visuals/Axis/NumericAxis";
import { SciChartSurface } from "scichart";
import { UniformHeatmapDataSeries } from "scichart/Charting/Model/UniformHeatmapDataSeries";
import { UniformHeatmapRenderableSeries } from "scichart/Charting/Visuals/RenderableSeries/UniformHeatmapRenderableSeries";
import { HeatmapColorMap } from "scichart/Charting/Visuals/RenderableSeries/HeatmapColorMap";
import { ZoomPanModifier } from "scichart/Charting/ChartModifiers/ZoomPanModifier";
import { ZoomExtentsModifier } from "scichart/Charting/ChartModifiers/ZoomExtentsModifier";
import { MouseWheelZoomModifier } from "scichart/Charting/ChartModifiers/MouseWheelZoomModifier";
import { zeroArray2D } from "scichart/utils/zeroArray2D";

const divElementId = "chart";

const MINIMUM_Z = 0.0;
const MAXIMUM_Z = 100.0;

//const REAL_TIME_MINIMUM_Z = 0.0;
//const REAL_TIME_MAXIMUM_Z = 50.0;

const WIDTH = 600;
const HEIGHT = 400;

const drawExperiment = async () => {
  const { sciChartSurface, wasmContext } = await SciChartSurface.create(divElementId);
  sciChartSurface.xAxes.add(new NumericAxis(wasmContext));
  sciChartSurface.yAxes.add(new NumericAxis(wasmContext));

  // Create a Heatmap Data-series. Pass heatValues as a number[][] to the UniformHeatmapDataSeries
  const initialZValues: number[][] = dataGenerator(HEIGHT, WIDTH, MINIMUM_Z, MAXIMUM_Z);
  const heatmapDataSeries = new UniformHeatmapDataSeries(wasmContext, 100, 1, 100, 1, initialZValues);
  
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

  // Add interaction
  sciChartSurface.chartModifiers.add(new ZoomPanModifier());
  sciChartSurface.chartModifiers.add(new ZoomExtentsModifier());
  sciChartSurface.chartModifiers.add(new MouseWheelZoomModifier());

  return { sciChartSurface, wasmContext, heatmapDataSeries };
}

function getRandomInteger(minimum: number, maximum: number): number {
  minimum = Math.ceil(minimum);
  maximum = Math.floor(maximum);
  // The maximum is exclusive and the minimum is inclusive
  return Math.floor(Math.random() * (maximum - minimum)) + minimum;
}

function dataGenerator(columns: number, rows: number, minimum: number, maximum: number): number[][] {
  const result = zeroArray2D([rows, columns]);
  for(let row = 0; row < rows; row++) {
    for(let col = 0; col < columns; col++) {
      result[row][col] = getRandomInteger(minimum, maximum);
    }
  }
  return result;
}

export default function Heatmap() {
  const [heatmapDataSeries, setHeatmapDataSeries] = React.useState<UniformHeatmapDataSeries>();
  const [sciChartSurface, setSciChartSurface] = React.useState<SciChartSurface>();

  React.useEffect(() => {
    (async () => {
      const res = await drawExperiment();
      setSciChartSurface(res.sciChartSurface);
      setHeatmapDataSeries(res.heatmapDataSeries);
    })();
    // Delete sciChartSurface on unmount component to prevent memory leak
    return () => sciChartSurface?.delete();
  }, []);

  const handleStart = () => {
    console.log("Start button clicked");
  };
  
  const handleStop = () => {
    console.log("Stop button clicked");
  };

  return (
    <div>
      <h1>SciChart JS Heatmap Experiment</h1>

      <div id={divElementId} style={{ maxWidth: 900 }}></div>

      <div id="performance-load-result-id"></div>

      <p>
        <button onClick={handleStart}>Start</button>
        <button onClick={handleStop}>Stop</button>
      </p>

      <div id="performance-timer-result-id"></div>
      <div id="performance-interval-result-id"></div>

    </div>
  );
}
