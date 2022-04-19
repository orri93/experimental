import { Component, Input, OnInit } from '@angular/core';

import { GraphService } from './../graph.service';

@Component({
  selector: 'app-m1c1',
  templateUrl: './m1c1.component.html',
  styleUrls: ['./m1c1.component.scss']
})
export class M1c1Component implements OnInit {
  @Input() ticks!: GraphTickValues;
  @Input() range!: GraphRange;
  @Input() scale!: GraphSize;

  componentName = 'Module no. 1 - Component no. 1';

  private d3GraphSvg: any;
  private xScale: any;
  private yScale: any;
  private size!: GraphSize;

  constructor(private graphService: GraphService) { }

  private defaultSize(): void {
    if (!this.ticks) {
      this.ticks = {
        x: [-2.0, -1.0, 1.0, 2.0],
        y: [-2.0, -1.0, 1.0, 2.0]
      }
    }
    if (!this.range) {
      this.range = {
        x: { from: -2.5, to: 2.5 },
        y: { from: -2.5, to: 2.5 }
      };
    }
    if (!this.scale) {
      this.scale = {
        width: 100,
        height: 100
      }
    }
  }

  draw(points: Point[]): void {
    this.graphService.draw(points);
  }

  clear(): void {
    this.graphService.clear();
  }

  ngOnInit(): void {
    this.defaultSize();
    this.graphService.initialize('graph', this.ticks, this.range, this.scale);
    this.graphService.drawAxes();
  }
}
