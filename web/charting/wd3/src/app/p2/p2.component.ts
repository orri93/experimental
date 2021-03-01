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

  size: ChartSize = { width: 256, height: 256 };

  constructor() { }

  resizeChild(): void {
    console.log('Resize Child clicked');
    const newSize: ChartSize = { width: 800, height: 400 };
    this.m1c2.resizeWebAssembly(newSize);
  }

  ngOnInit(): void {
  }
}
