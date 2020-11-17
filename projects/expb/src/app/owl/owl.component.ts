import { Component, ViewChild, ElementRef, NgZone, OnDestroy } from '@angular/core';
import { EmscriptenWasmComponent } from '../emscripten-wasm.component';

const RES_X = 600.0;
const RES_Y = 400.0;

const requestFullscreen =
  document.documentElement.requestFullscreen ||
  document.documentElement["webkitRequestFullscreen"] ||
  document.documentElement["msRequestFullscreen"] ||
  document.documentElement["mozRequestFullScreen"];

@Component({
  selector: 'app-owl',
  templateUrl: './owl.component.html',
  styleUrls: ['./owl.component.css']
})
export class OwlComponent extends EmscriptenWasmComponent implements OnDestroy {
  @ViewChild("canvas") canvas: ElementRef;

  title = 'Owl';

  error: string;
  supportsFullscreen: boolean;

  constructor(private ngZone: NgZone) {
    super("OwlModule", "owl.js");

    this.supportsFullscreen = !!requestFullscreen;

    this.moduleDecorator = (mod) => {
      mod.arguments = [];
      mod.preRun = [];
      mod.canvas = <HTMLCanvasElement>this.canvas.nativeElement;
      mod.printErr = (what: string) => {
        if (!what.startsWith("WARNING")) {
          this.ngZone.run(() => (this.error = what));
        }
      };
    };
  }

  toggleFullscreen() {
    if (requestFullscreen) {
      requestFullscreen.bind(this.canvas.nativeElement)();
    }
  }

  start(): void {
    console.log("Start");
  }
  
  stop(): void {
    console.log("Stop Button Clicked");
  }

  ngOnDestroy(): void {
    console.log("On ng destroy");
  }
}
