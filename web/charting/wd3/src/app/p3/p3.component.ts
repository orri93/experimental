import { Component, ViewChild, ChangeDetectorRef, OnInit } from '@angular/core';
import * as _ from 'lodash-es';
import { M1c3Component } from './../m1/m1c3/m1c3.component';

@Component({
  selector: 'app-p3',
  templateUrl: './p3.component.html',
  styleUrls: ['./p3.component.scss']
})
export class P3Component implements OnInit {
  @ViewChild('m1c3') m1c3: M1c3Component;

  pageName = 'Page no. 3';

  childSize: ChartSize = { width: 0, height: 0};

  constructor(private changeDetector: ChangeDetectorRef) { }

  childSizeChanged(e: ChartSize): void {
    // console.log('On Child Size Changed');
    this.childSize.width = e.width;
    this.childSize.height = e.height;
    this.changeDetector.detectChanges();
  }

  start(): void {
    console.log('Start clicked');
  }

  stop(): void {
    console.log('Stop clicked');
  }

  ngOnInit(): void {
  }
}
