import { Component, ViewChild, OnInit } from '@angular/core';

import { M1c2Component } from './../m1/m1c2/m1c2.component';

@Component({
  selector: 'app-p2',
  templateUrl: './p2.component.html',
  styleUrls: ['./p2.component.scss']
})
export class P2Component implements OnInit {
  @ViewChild('m1c2') m1c2: M1c2Component;

  pageName = 'Page no. 2';

  size: ChartSize = { width: 800, height: 400 };

  constructor() { }

  start(): void {
    console.log('Start clicked');
  }

  stop(): void {
    console.log('Stop clicked');
  }

  ngOnInit(): void {
  }
}
