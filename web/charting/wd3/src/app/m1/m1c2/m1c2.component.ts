import { Component, Input, ViewChild, ElementRef, NgZone, SimpleChanges, OnChanges,  OnDestroy } from '@angular/core';

import { EmscriptenWasmComponentDirective } from './../../emscripten-wasm.component';

@Component({
  selector: 'app-m1c2',
  templateUrl: './m1c2.component.html',
  styleUrls: ['./m1c2.component.scss']
})
export class M1c2Component extends EmscriptenWasmComponentDirective implements OnChanges, OnDestroy {
  @ViewChild('canvas') canvas: ElementRef;
  @ViewChild('demodiv') demo: ElementRef;
  @Input() size: ChartSize;

  componentName = 'Module no. 1 - Component no. 2';

  error: string;

  constructor(private ngZone: NgZone) {
    super('DemoModule', 'demo.js');
    const self = this;
    this.moduleDecorator = (mod) => {
      mod.arguments = [
        self.size.width.toString(),
        self.size.height.toString()
      ];
      mod.preRun = [];
      mod.canvas = this.canvas.nativeElement as HTMLCanvasElement;
      mod.printErr = (what: string) => {
        if (!what.startsWith('WARNING')) {
          this.ngZone.run(() => (self.error = what));
        }
      };
    };
  }

  ngOnChanges(changes: SimpleChanges): void {
    for (const change in changes) {
      if (change === 'size') {
        console.log('Update the size');
      }
    }
  }

  ngOnDestroy(): void {
    if (super.module) {
      console.log('Calling shutdown in module');
      super.module.ccall('shutdown', 'void', [], []);
    }
  }
}
