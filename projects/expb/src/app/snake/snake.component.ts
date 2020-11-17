import { Component, ViewChild, ElementRef, NgZone, OnDestroy } from '@angular/core';
import { EmscriptenWasmComponent } from '../emscripten-wasm.component';

const requestFullscreen =
  document.documentElement.requestFullscreen ||
  document.documentElement["webkitRequestFullscreen"] ||
  document.documentElement["msRequestFullscreen"] ||
  document.documentElement["mozRequestFullScreen"];

@Component({
  selector: 'app-snake',
  templateUrl: './snake.component.html',
  styleUrls: ['./snake.component.css']
})
export class SnakeComponent extends EmscriptenWasmComponent implements OnDestroy {
  @ViewChild("canvas") canvas: ElementRef;

  title = 'Snake';

  error: string;
  supportsFullscreen: boolean;

  constructor(private ngZone: NgZone) {
    super("SnakeModule", "snake.js");

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
