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

  constructor() {}

  ngOnInit(): void {
  }
}
