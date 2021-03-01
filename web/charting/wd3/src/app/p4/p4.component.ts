import { Component, ViewChild, ChangeDetectorRef } from '@angular/core';
import * as _ from 'lodash-es';
import { M1c4Component } from './../m1/m1c4/m1c4.component';

@Component({
  selector: 'app-p4',
  templateUrl: './p4.component.html',
  styleUrls: ['./p4.component.scss']
})
export class P4Component {
  @ViewChild('m1c4') m1c4: M1c4Component;

  pageName = 'Page no. 4';

  childSize: ChartSize = { width: 0, height: 0};

  constructor(private changeDetector: ChangeDetectorRef) { }

  childSizeChanged(e: ChartSize): void {
    // console.log('On Child Size Changed');
    this.childSize.width = e.width;
    this.childSize.height = e.height;
    this.changeDetector.detectChanges();
  }
}
