import { Component, OnInit } from '@angular/core';

import * as d3 from 'd3';

// D3.Js: Visualize a Mathematical Function
// https://blog.devgenius.io/d3-js-visualize-a-mathematical-function-d0c164ddcf2c

// Convention: https://bl.ocks.org/mbostock/3019563
const margin = { top: 10, right: 50, bottom: 50, left: 50 },
  width = 450 - margin.left - margin.right,
  height = 400 - margin.top - margin.bottom;

@Component({
  selector: 'app-p1',
  templateUrl: './p1.component.html',
  styleUrls: ['./p1.component.scss']
})
export class P1Component implements OnInit {

  pageName = 'Page no. 1';

  constructor() { }

  f(x: number): number {
    return 2.0 * x + 1.0;
  }

  create(numberOfPoints: number) {
    const data = [];
    for (var i = 0; i < numberOfPoints; i++) {
      const x = i / 100.0;
      const y = this.f(x);
      data.push([x, y]);
    }
  }

  ngOnInit(): void {
    const svg = d3.select("#root").attr("width", width + margin.left + margin.right)
      .attr("height", height + margin.top + margin.bottom)
      .append("g")
      .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

    // Define chart area
    svg
      .append("clipPath")
      .attr("id", "chart-area")
      .append("rect")
      .attr("x", 0)
      .attr("y", 0)
      .attr("width", width)
      .attr("height", height);

    // Add Axes
    const xMax = 4;
    const yMax = 5;

    const xScale = d3.scaleLinear([0, xMax], [0, width]);
    const yScale = d3.scaleLinear([0, yMax], [height, 0]);

    const xAxis = d3.axisBottom(xScale);
    const yAxis = d3.axisLeft(yScale);

    svg.append("g")
      .attr("transform", `translate(0,${height})`)
      .call(xAxis);
    svg.append("g")
      .attr("transform", `translate(0,0)`)
      .call(yAxis);

    // Axes label
    svg.append("text")
      .attr("class", "x label")
      .attr("text-anchor", "end")
      .attr("x", width / 2 + 5)
      .attr("y", height + 35)
      .text("x");

    svg.append("text")
      .attr("class", "y label")
      .attr("text-anchor", "end")
      .attr("y", -35)
      .attr("x", -height / 2)
      .attr("transform", "rotate(-90)")
      .html("y");

    const data = this.create(200);


  }
}
