import { Component, OnInit } from '@angular/core';
import { AppConfiguration } from './../app.configuration';

import * as d3 from 'd3';

@Component({
  selector: 'app-heatmap',
  templateUrl: './heatmap.component.html',
  styleUrls: ['./heatmap.component.css']
})
export class HeatmapComponent implements OnInit {
  title = 'Heatmap';

  private data = [];
  private svg;

  constructor() {}

  private createData(): void {
    const range = AppConfiguration.settings.heatmap.range;
    const dz = range.z.to - range.z.from;
    console.time('Create Data');
    for (let i = range.x.from; i < range.x.to; i++) {
      for (let j = range.y.from; j < range.y.to; j++) {
        const k = range.z.from + ((i * j) % dz);
        this.data.push({x: i, y: j, z: k });
      }
    }
    console.timeEnd('Create Data');
  }

  private createSvg(): void {
    const chart = AppConfiguration.settings.heatmap.chart;
    const margin = chart.margin;
    this.svg = d3.select('figure#heatmap-figure')
      .append('svg')
      .attr('width', chart.width)
      .attr('height', chart.height)
      .append('g')
      .attr('transform', 'translate(' + margin + ',' + margin + ')');
  }

  private drawPlot(): void {
    const chart = AppConfiguration.settings.heatmap.chart;
    const range = AppConfiguration.settings.heatmap.range;
    const margin = chart.margin;
    const width = chart.width - (margin * 2);
    const height = chart.height - (margin * 2);

    const rectWidth = width / (range.x.to - range.x.from);
    const rectHeight = height / (range.y.to - range.y.from);

    // Add X axis
    const xAxis = d3.scaleLinear()
      .domain([range.x.from, range.x.to])
      .range([ 0, width ]);
    this.svg.append('g')
      .attr('transform', 'translate(0,' + height + ')')
      .call(d3.axisBottom(xAxis).tickFormat(d3.format('d')));

    // Add Y axis
    const yAxis = d3.scaleLinear()
      .domain([range.y.from, range.y.to])
      .range([ height, 0]);
    this.svg.append('g')
      .call(d3.axisLeft(yAxis));

    // Add Z axis
    const zAxis = d3.scaleSequential(d3.interpolateBlues)
      .domain([range.z.from, range.z.to]);

    console.time('Create Dots');

    const dots = this.svg.append('g');

    dots.selectAll('dot')
      .data(this.data)
      .enter()
      .append('rect')
      .attr('width', rectWidth)
      .attr('height', rectHeight)
      .attr('x', d => xAxis(d.x))
      .attr('y', d => yAxis(d.y))
      .attr('fill', d => zAxis(d.z));

    console.timeEnd('Create Dots');
  }

  ngOnInit(): void {
    this.createData();
    this.createSvg();
    this.drawPlot();
  }
}
