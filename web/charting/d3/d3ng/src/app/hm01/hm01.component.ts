import { Component, ViewChild, ElementRef, NgZone, OnDestroy } from '@angular/core';
import { EmscriptenWasmComponent } from '../emscripten-wasm.component';

const requestFullscreen =
  document.documentElement.requestFullscreen ||
  document.documentElement['webkitRequestFullscreen'] ||
  document.documentElement['msRequestFullscreen'] ||
  document.documentElement['mozRequestFullScreen'];

@Component({
  selector: 'app-hm01',
  templateUrl: './hm01.component.html',
  styleUrls: ['./hm01.component.css']
})
export class Hm01Component extends EmscriptenWasmComponent implements OnDestroy {
  @ViewChild('canvas') canvas: ElementRef;

  title = 'Mandelbrot set';
  
  error: string;
  supportsFullscreen: boolean;

  constructor(private ngZone: NgZone) {
    super('HmModule', 'hm.js');

    this.supportsFullscreen = !!requestFullscreen;

    this.moduleDecorator = (mod) => {
      mod.arguments = ['640', '400'];
      mod.preRun = [];
      mod.canvas = <HTMLCanvasElement>this.canvas.nativeElement;
      mod.printErr = (what: string) => {
        if (!what.startsWith("WARNING")) {
          this.ngZone.run(() => (this.error = what));
        }
      };
    };
  }

  ngOnDestroy(): void {
    console.log('On ng destroy');
    if(this.module) {
      console.log("Calling shutdown in module");
      this.module.ccall("shutdown", "void", [], []);
    }
  }
}
