import * as _ from 'lodash';
import * as moment from 'moment';
import { Component, OnInit, ViewChild } from '@angular/core';

import { HmcChartComponent } from '../hmc-chart/hmc-chart.component';
import { Hmco } from './options/hmco.options.datamodel';



@Component({
  selector: 'app-hmc',
  templateUrl: './hmc.component.html',
  styleUrls: ['./hmc.component.css']
})
export class HmcComponent implements OnInit {
  @ViewChild(HmcChartComponent) chart: HmcChartComponent;

  public hmcOptions: Hmco;
  public toolCount: number;
  public tool1Tool2DeltaColor: string;
  public tool2Tool3DeltaColor: string;
  public tool3Tool4DeltaColor: string;
  public tool4Tool5DeltaColor: string;
  public ecdLowerLimit: number;
  public ecdUpperLimit: number;

  public depthUnitAbbr: string;
  public ecdUnitAbbr: string;
  public displayToolNames: string[];
  public displayDepth: number[];
  public displayEcd: number[];

  constructor() { }

  ngOnInit(): void {
  }

}
