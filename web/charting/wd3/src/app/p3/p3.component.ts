import { Component, ViewChild, OnInit } from '@angular/core';

import { M1c3Component } from './../m1/m1c3/m1c3.component';

@Component({
  selector: 'app-p3',
  templateUrl: './p3.component.html',
  styleUrls: ['./p3.component.scss']
})
export class P3Component implements OnInit {
  @ViewChild('m1c3') m1c3: M1c3Component;

  pageName = 'Page no. 3';

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
