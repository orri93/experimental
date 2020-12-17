import { Component, OnInit } from '@angular/core';
import { AppConfiguration } from './../app.configuration';
import * as d3 from 'd3';

// Data visualization in Angular using D3.js
// https://blog.logrocket.com/data-visualization-angular-d3/

@Component({
  selector: 'app-pie',
  templateUrl: './pie.component.html',
  styleUrls: ['./pie.component.css']
})
export class PieComponent implements OnInit {
  title = 'Pie';

  private data = [
    {"Framework": "Vue", "Stars": "166443", "Released": "2014"},
    {"Framework": "React", "Stars": "150793", "Released": "2013"},
    {"Framework": "Angular", "Stars": "62342", "Released": "2016"},
    {"Framework": "Backbone", "Stars": "27647", "Released": "2010"},
    {"Framework": "Ember", "Stars": "21471", "Released": "2011"},
  ];

  private svg;

  private colors;

  constructor() { }

  private createSvg(): void {
    let width = AppConfiguration.settings.pie.chart.width;
    let height = AppConfiguration.settings.pie.chart.height;
    this.svg = d3.select("figure#pie-figure")
    .append("svg")
    .attr("width", width)
    .attr("height", height)
    .append("g")
    .attr(
      "transform",
      "translate(" + width / 2 + "," + height / 2 + ")"
    );
  }

  private createColors(): void {
    this.colors = d3.scaleOrdinal()
    .domain(this.data.map(d => d.Stars.toString()))
    .range(["#c7d3ec", "#a5b8db", "#879cc4", "#677795", "#5a6782"]);
  }

  private drawChart(): void {
    let margin = AppConfiguration.settings.pie.chart.margin;
    let width = AppConfiguration.settings.pie.chart.width;
    let height = AppConfiguration.settings.pie.chart.height;

    // The radius of the pie chart is half the smallest side
    let radius = Math.min(width, height) / 2 - margin;

    // Compute the position of each group on the pie:
    const pie = d3.pie<any>().value((d: any) => Number(d.Stars));

    // Build the pie chart
    this.svg
    .selectAll('pieces')
    .data(pie(this.data))
    .enter()
    .append('path')
    .attr('d', d3.arc()
      .innerRadius(0)
      .outerRadius(radius)
    )
    .attr('fill', (d, i) => (this.colors(i)))
    .attr("stroke", "#121926")
    .style("stroke-width", "1px");

    // Add labels
    const labelLocation = d3.arc()
    .innerRadius(100)
    .outerRadius(radius);

    this.svg
    .selectAll('pieces')
    .data(pie(this.data))
    .enter()
    .append('text')
    .text(d => d.data.Framework)
    .attr("transform", d => "translate(" + labelLocation.centroid(d) + ")")
    .style("text-anchor", "middle")
    .style("font-size", 15);
  }

  ngOnInit(): void {
    this.createSvg();
    this.createColors();
    this.drawChart();
  }
}
