import { Component, OnInit } from '@angular/core';
import { AppConfiguration } from './../app.configuration';
import * as d3 from 'd3';

// Data visualization in Angular using D3.js
// https://blog.logrocket.com/data-visualization-angular-d3/

@Component({
  selector: 'app-bar',
  templateUrl: './bar.component.html',
  styleUrls: ['./bar.component.css']
})
export class BarComponent implements OnInit {
  title = 'Bar';

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
    let margin = AppConfiguration.settings.bar.chart.margin;
    this.svg = d3.select("figure#bar-figure")
      .append('svg')
      .attr('width', AppConfiguration.settings.bar.chart.width)
      .attr('height', AppConfiguration.settings.bar.chart.height)
      .append('g')
      .attr('transform', 'translate(' + margin + ',' + margin + ')');
  }

  private drawBars(data: any[]): void {
    let margin = AppConfiguration.settings.bar.chart.margin;
    let width = AppConfiguration.settings.bar.chart.width - (margin * 2);
    let height = AppConfiguration.settings.bar.chart.height - (margin * 2);

    // Add X axis
    const x = d3.scaleBand()
      .range([0, width])
      .domain(data.map(d => d.Framework))
      .padding(0.2);

    this.svg.append("g")
      .attr("transform", "translate(0," + height + ")")
      .call(d3.axisBottom(x))
      .selectAll("text")
      .attr("transform", "translate(-10,0)rotate(-45)")
      .style("text-anchor", "end");

    // Add Y axis
    const y = d3.scaleLinear()
      .domain([0, 200000])
      .range([height, 0]);

    this.svg.append("g")
      .call(d3.axisLeft(y));

    // Create and fill the bars
    this.svg.selectAll("bars")
      .data(data)
      .enter()
      .append("rect")
      .attr("x", d => x(d.Framework))
      .attr("y", d => y(d.Stars))
      .attr("width", x.bandwidth())
      .attr("height", (d) => height - y(d.Stars))
      .attr("fill", "#d04a35");
  }

  ngOnInit(): void {
    this.createSvg();
    this.drawBars(this.data);
  }
}
