import { Component, OnInit } from '@angular/core';
import { AppConfiguration } from './../app.configuration';
import * as d3 from 'd3';

// Data visualization in Angular using D3.js
// https://blog.logrocket.com/data-visualization-angular-d3/

@Component({
  selector: 'app-scatter',
  templateUrl: './scatter.component.html',
  styleUrls: ['./scatter.component.css']
})
export class ScatterComponent implements OnInit {
  title = 'Scatter';

  private data = [
    {"Framework": "Vue", "Stars": "166443", "Released": "2014"},
    {"Framework": "React", "Stars": "150793", "Released": "2013"},
    {"Framework": "Angular", "Stars": "62342", "Released": "2016"},
    {"Framework": "Backbone", "Stars": "27647", "Released": "2010"},
    {"Framework": "Ember", "Stars": "21471", "Released": "2011"},
  ];
  private svg;

  constructor() { }

  private createSvg(): void {
    let margin = AppConfiguration.settings.scatter.chart.margin;
    this.svg = d3.select("figure#scatter-figure")
      .append("svg")
      .attr("width", AppConfiguration.settings.scatter.chart.width)
      .attr("height", AppConfiguration.settings.scatter.chart.height)
      .append("g")
      .attr("transform", "translate(" + margin + "," + margin + ")");
  }

  private drawPlot(): void {
    let margin = AppConfiguration.settings.scatter.chart.margin;
    let width = AppConfiguration.settings.scatter.chart.width - (margin * 2);
    let height = AppConfiguration.settings.scatter.chart.height - (margin * 2);

    // Add X axis
    const x = d3.scaleLinear()
      .domain([2009, 2017])
      .range([ 0, width ]);
    this.svg.append("g")
      .attr("transform", "translate(0," + height + ")")
      .call(d3.axisBottom(x).tickFormat(d3.format("d")));

    // Add Y axis
    const y = d3.scaleLinear()
      .domain([0, 200000])
      .range([ height, 0]);
    this.svg.append("g")
      .call(d3.axisLeft(y));

    // Add dots
    const dots = this.svg.append('g');

    dots.selectAll("dot")
      .data(this.data)
      .enter()
      .append("circle")
      .attr("cx", d => x(d.Released))
      .attr("cy", d => y(d.Stars))
      .attr("r", 7)
      .style("opacity", .5)
      .style("fill", "#69b3a2");

    dots.selectAll("text")
      .data(this.data)
      .enter()
      .append("text")
      .text(d => d.Framework)
      .attr("x", d => x(d.Released))
      .attr("y", d => y(d.Stars));
  }

  ngOnInit(): void {
    this.createSvg();
    this.drawPlot();
  }
}
