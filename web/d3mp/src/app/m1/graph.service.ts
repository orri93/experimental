import { Injectable } from '@angular/core';

import * as d3 from 'd3';

@Injectable({
  providedIn: 'root'
})
export class GraphService {

  ticks!: GraphTickValues;

  private id!: string;

  private range!: GraphRange;
  private size: GraphSize;

  private xScale: any;
  private yScale: any;

  constructor() {
    this.size = {width: 0, height: 0};
  }

  private select(id: string): any {
    return d3.select('svg#' + this.id);
  }

  calculateSize(range: GraphRange, scale: GraphSize): void {
    this.range = range;
    this.size = {
      width: scale.width * (this.range.x.to - this.range.x.from),
      height: scale.height * (this.range.y.to - this.range.y.from)
    };
  }

  selectGraph(id: string): void {
    this.id = id;
    this.select(this.id)
      .attr('width', this.size.width)
      .attr('height', this.size.height);
  }

  createScales(): void {
    this.xScale = d3.scaleLinear()
      .domain([this.range.x.from, this.range.x.to])
      .range([0, this.size.width]);

    this.yScale = d3.scaleLinear()
      .domain([this.range.y.from, this.range.y.to])
      .range([this.size.height, 0]);
  }

  drawAxes(): void {
    const axes = { x: 0, y: 0 }
    const d3GraphSvg = this.select(this.id);
    d3GraphSvg.append('g')
      .attr('id', 'xag')
      .attr('transform', 'translate(' + axes.x + ',' + (this.size.height / 2) + ')')
      .call(d3.axisBottom(this.xScale).tickValues(this.ticks.x));
    d3GraphSvg.append('g')
      .attr('id', 'yag')
      .attr('transform', 'translate(' + (this.size.width / 2) + ', ' +  axes.y + ')')
      .call(d3.axisLeft(this.yScale).tickValues(this.ticks.y));
  }

  initialize(id: string, ticks: GraphTickValues, range: GraphRange, scale: GraphSize): void {
    this.ticks = ticks;
    this.calculateSize(range, scale);
    this.selectGraph(id);
    this.createScales();
  }

  draw(points: Point[]): void {
    const d3GraphSvg = this.select(this.id);
    d3GraphSvg.selectAll('dot')
      .data(points).enter().append('circle')
      .attr("r", 1.0)
      .attr("cx", (d: Point) => this.xScale(d.x))
      .attr("cy", (d: Point) => this.yScale(d.y));
  }

  clear(): void {
    const d3GraphSvg = this.select(this.id);
    d3GraphSvg.selectAll('circle').remove();
  }
}
