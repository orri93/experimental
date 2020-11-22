import { Component, ViewChild, ElementRef, NgZone, OnDestroy } from '@angular/core';
import { EmscriptenWasmComponent } from '../emscripten-wasm.component';

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

  ngOnDestroy(): void {
    console.log("On ng destroy");
    if(this.module) {
      console.log("Calling shutdown in module");
      this.module.ccall("shutdown", "void", [], []);
    }
  }
}
