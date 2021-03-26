import { Component, ViewChild, ChangeDetectorRef } from '@angular/core';
import * as _ from 'lodash-es';
import { AppConfiguration } from './../app.configuration';
import { M4c2Component } from './../m4/m4c2/m4c2.component';

@Component({
  selector: 'app-p6',
  templateUrl: './p6.component.html',
  styleUrls: ['./p6.component.scss']
})
export class P6Component {
  @ViewChild('m4c2') m4c2: M4c2Component;

  pageName = 'Page no. 6';

  childSize: ChartSize = { width: 0, height: 0};

  constructor(private changeDetector: ChangeDetectorRef) { }
  
  childSizeChanged(size: ChartSize): void {
    console.log('On Child Size Changed');
    this.childSize = _.clone(size);
    this.changeDetector.detectChanges();
  }
}
